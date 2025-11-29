/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipeline.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:59:11 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 13:54:47 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	close_unused_pipe_fd(int (*pipes)[2], int n, int keep_in_fd, int keep_out_fd)
{
	int	i;

	if (!pipes || n <= 1)
		return ;
	i = 0;
	while (i < n - 1)
	{
		if (pipes[i][0] >= 0 && pipes[i][0] != keep_in_fd)
		{
			close(pipes[i][0]);
		}
		if (pipes[i][1] >= 0 && pipes[i][1] != keep_out_fd)
		{
			close(pipes[i][1]);
		}
		i++;
	}
}

static void child_exec(t_ast *cmd, char **envp)
{
    char    *path;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        _exit(127);
    path = find_path((char *)cmd->argv[0], envp);
    if (!path)
    {
        exec_error("command not found", cmd->argv[0]);
        _exit(127);
    }
    execve(path, cmd->argv, envp);
    if (errno == EACCES)
        exec_error("permission denied", cmd->argv[0]);
    else
        exec_error("execve failed", cmd->argv[0]);
    free(path);
    if (errno == EACCES)
        _exit (126);
    else
        _exit(127);
}

static void child_reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

pid_t    spawn_child(t_ast *cmd, int in_fd, int out_fd, t_data *data)
{
    pid_t   pid;

    pid = fork();
    if (pid < 0)
        return (-1);
    if (pid == 0)
    {
        child_reset_signals();
		if (data)
			close_unused_pipe_fd(data->pipes, data->pipes_count, in_fd, out_fd);
        if (in_fd != -1 && dup2(in_fd, STDIN_FILENO) == -1)
			_exit(127);
		if (out_fd != -1 && dup2(out_fd, STDOUT_FILENO) == -1)
			_exit(127);
		if (out_fd != -1 && dup2(out_fd, STDERR_FILENO) == -1)
            _exit(127);
		if (in_fd != -1 && in_fd != STDIN_FILENO)
			close(in_fd);
		if (out_fd != -1 && out_fd != STDOUT_FILENO && out_fd != STDERR_FILENO)
			close(out_fd);
		if (data)
			child_exec(cmd, data->envp);
		else
			child_exec(cmd, NULL);
        _exit(127);
    }
    return (pid);
}
/* tremenda fumada me he pegado.. esta funciion es inutil, bueno, sirve para el multipipe
me he liado a crear pipes cuando solo debia llamar a exec_comand... tecnoooloooogiiiaa...

int run_single(t_ast *cmd, t_data *data)
{
    int     pipefd[2];
    pid_t   pid;
    int     status;

    if (pipe(pipefd) == -1)
        return (exec_command(cmd, data));
    pid = spawn_child(cmd, data->envp, -1, pipefd[1]);
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (127);
    }
    close(pipefd[1]);
    forward_fd(pipefd[0]);
    status = wait_for_all(&pid, 1);
    return (status);
}
*/

int run_single(t_ast *cmd, t_data *data)
{
    (void)data;
    return (exec_command(cmd, data));
}
