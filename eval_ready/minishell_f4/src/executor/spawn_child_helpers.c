/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_child_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:59:11 by jadelgad          #+#    #+#             */
/*   Updated: 2025/12/03 13:19:29 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	close_unused_pipe_fd(int (*pipes)[2], int n,
	int keep_in_fd, int keep_out_fd)
{
	int	i;

	if (!pipes || n <= 1)
		return ;
	i = 0;
	while (i < n - 1)
	{
		if (pipes[i][0] >= 0 && pipes[i][0] != keep_in_fd)
			close(pipes[i][0]);
		if (pipes[i][1] >= 0 && pipes[i][1] != keep_out_fd)
			close(pipes[i][1]);
		i++;
	}
}

void	setup_child_io(t_data *data, int in_fd, int out_fd)
{
	reset_sig_in_child();
	if (data)
		close_unused_pipe_fd(data->pipes, data->pipes_count, in_fd, out_fd);
	if (in_fd != -1)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			exit(127);
	}
	if (out_fd != -1)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			exit(127);
	}
	if (in_fd != -1 && in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
		close(out_fd);
}

static void	exec_external_and_exit(t_ast *cmd)
{
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		exit(127);
	path = find_path((char *)cmd->argv[0], NULL);
	if (!path)
	{
		exec_error("command not found", cmd->argv[0]);
		exit(127);
	}
	execve(path, cmd->argv, NULL);
	if (errno == EACCES)
		exec_error("permission denied", cmd->argv[0]);
	else
		exec_error("execve failed", cmd->argv[0]);
	free(path);
	if (errno == EACCES)
		exit(126);
	else
		exit(127);
}

void	run_child_and_exit(t_ast *cmd, t_data *data)
{
	int	status;

	if (data)
	{
		if (cmd->type == AST_REDIRECT)
		{
			apply_redirect_and_exec_child(cmd, data);
			exit(1);
		}
		status = exec_ast_in_child(cmd, data);
		exit(status);
	}
	exec_external_and_exit(cmd);
}
