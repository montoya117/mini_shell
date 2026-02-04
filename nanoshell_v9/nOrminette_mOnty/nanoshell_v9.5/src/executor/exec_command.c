/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:58:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 13:33:53 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// Formato: "bash: line 1: <cmd>: <msg>\n"
static void	bash_style_error(const char *cmd, const char *msg)
{
	if (!cmd || !msg)
		return ;
	write(2, "bash: line 1: ", 14);
	write(2, cmd, ft_strlen_const(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen_const(msg));
	write(2, "\n", 1);
}

int	wait_for_child(pid_t pid)
{
	int		status;
	int		ret;
	pid_t	w;

	while (1)
	{
		w = waitpid(pid, &status, 0);
		if (w == -1)
		{
			if (errno == EINTR)
				continue ;
			perror("waitpid");
			return (127);
		}
		break ;
	}
	if (WIFEXITED(status))
		ret = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		ret = 128 + WTERMSIG(status);
	else
		ret = status;
	return (ret);
}
// +128 para distinguir fallo por señal \ fallo por exit code.

void	reset_sig_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

static t_ast	*get_core_node(t_ast *node)
{
	t_ast	*curr;

	curr = node;
	while (curr && curr->type == AST_REDIRECT)
		curr = curr->left;
	return (curr);
}

static char	**prepare_env(t_ast *node, t_data *data)
{
	char	**env;

	env = data->envp;
	if (node->assignments && node->assignments[0])
	{
		env = dup_env(data->envp);
		if (!env)
			exit(1);
		apply_assignments_array(&env, node->assignments);
	}
	return (env);
}

static void	handle_exec_error(char *cmd, char *path, char **env, t_data *data)
{
	int	exit_code;

	exit_code = 127;
	if (errno == EACCES || errno == ENOEXEC)
	{
		bash_style_error(cmd, strerror(errno));
		exit_code = 126;
	}
	else if (errno != 0)
		bash_style_error(cmd, strerror(errno));
	if (path)
		free(path);
	if (env != data->envp)
		free_env(env);
	exit(exit_code);
}

static void	exec_child_builtin(t_ast *node, t_data *data, char **env)
{
	int	status;

	reset_sig_in_child();
	status = call_builtin(node->argv, data);
	if (env != data->envp)
		free_env(env);
	exit(status);
}

void	exec_child(t_ast *node, t_data *data)
{
	char	**env;
	char	*path;
	t_ast	*cmd;

	if (apply_all_redirections(node) == -1)
		exit(1);
	cmd = get_core_node(node);
	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(0);
	env = prepare_env(cmd, data);
	if (is_builtin(cmd->argv[0])
		&& !builtin_should_run_in_parent(cmd->argv[0]))
		exec_child_builtin(cmd, data, env);
	path = find_path((char *)cmd->argv[0], env);
	if (!path)
	{
		bash_style_error(cmd->argv[0], "command not found");
		if (env != data->envp)
			free_env(env);
		exit(127);
	}
	reset_sig_in_child();
	execve(path, cmd->argv, env);
	handle_exec_error(cmd->argv[0], path, env, data);
}

static int	exec_builtin_parent(t_ast *node, t_data *data)
{
	t_ast	*core;
	int		save_fds[2];
	int		status;

	core = get_core_node(node);
	if (!core || !core->argv)
		return (0);
	save_fds[0] = dup(STDIN_FILENO);
	save_fds[1] = dup(STDOUT_FILENO);
	if (apply_all_redirections(node) == -1)
	{
		dup2(save_fds[0], STDIN_FILENO);
		dup2(save_fds[1], STDOUT_FILENO);
		return (close(save_fds[0]), close(save_fds[1]), 1);
	}
	status = call_builtin(core->argv, data);
	dup2(save_fds[0], STDIN_FILENO);
	dup2(save_fds[1], STDOUT_FILENO);
	close(save_fds[0]);
	close(save_fds[1]);
	return (status);
}

int	exec_command(t_ast *node, t_data *data)
{
	pid_t	pid;

	if (!node || !data)
		return (127);
	if (node->argv && node->argv[0] && is_builtin(node->argv[0])
		&& builtin_should_run_in_parent(node->argv[0]))
		return (exec_builtin_parent(node, data));
	pid = fork();
	if (pid < 0)
		return (perror("fork"), 127);
	if (pid == 0)
		exec_child(node, data);
	return (wait_for_child(pid));
}
