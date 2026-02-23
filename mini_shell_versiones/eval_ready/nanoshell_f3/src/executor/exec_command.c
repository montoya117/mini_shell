/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:58:42 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/11 13:41:53 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// Formato: "bash: line 1: <cmd>: <msg>\n"
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
		return (WEXITSTATUS(status));
	ret = 128 + WTERMSIG(status);
	if (WTERMSIG(status) == SIGINT)
		write(1, "\n", 1);
	else if (WTERMSIG(status) == SIGQUIT)
		write(1, "Quit (core dumped)\n", 19);
	return (ret);
}

void	reset_sig_in_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	exec_child(t_ast *node, t_data *data)
{
	char	**env;
	char	*path;
	t_ast	*cmd;

	reset_sig_in_child();
	data->pid = 1;
	if (apply_all_redirections(node) == -1)
	{
		rl_clear_history();
		exit(1);
	}
	cmd = get_core_node(node);
	if (!cmd || !cmd->argv || !cmd->argv[0])
	{
		rl_clear_history();
		exit(0);
	}
	env = prepare_env(cmd, data);
	if (is_builtin(cmd->argv[0]))
		exec_child_builtin(cmd, data, env);
	path = find_path((char *)cmd->argv[0], env);
	if (!path)
	{
		bash_style_error(cmd->argv[0], "command not found");
		if (env != data->envp)
			free_env(env);
		rl_clear_history();
		exit(127);
	}
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
