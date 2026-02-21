/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:58:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 13:33:53 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	exec_child_builtin(t_ast *node, t_data *data, char **env)
{
	int	status;

	reset_sig_in_child();
	status = call_builtin(node->argv, data);
	if (env != data->envp)
		free_env(env);
	rl_clear_history();
	exit(status);
}

void	bash_style_error(const char *cmd, const char *msg)
{
	if (!cmd || !msg)
		return ;
	write(2, "bash: line 1: ", 14);
	write(2, cmd, ft_strlen_const(cmd));
	write(2, ": ", 2);
	write(2, msg, ft_strlen_const(msg));
	write(2, "\n", 1);
}

t_ast	*get_core_node(t_ast *node)
{
	t_ast	*curr;

	curr = node;
	while (curr && curr->type == AST_REDIRECT)
		curr = curr->left;
	return (curr);
}

char	**prepare_env(t_ast *node, t_data *data)
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

void	handle_exec_error(char *cmd, char *path, char **env, t_data *data)
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
	rl_clear_history();
	exit(exit_code);
}
