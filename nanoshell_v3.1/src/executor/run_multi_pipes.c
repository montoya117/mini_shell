/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_multi_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:58:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 13:33:53 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void    parent_close_intermediates(int (*pipes)[2], int n)
{
    int i;

    i = 0;
    
    while (i < n - 1)
	{
    	if (pipes[i][1] >= 0)
			close(pipes[i][1]);
		if (i < n - 2 && pipes[i][0] >= 0)
			close(pipes[i][0]);
		i++;
	}
}

int  spawn_all_children(t_ast **cmds, int n, int (*pipes)[2], t_data *data, pid_t *pids)
{
    int i;
    int in_fd;
    int out_fd;

    i = 0;
    while (i < n)
    {
        in_fd = -1;
        out_fd = -1;
        if (i > 0)
            in_fd = pipes[i - 1][0];
        if (i < n - 1)
            out_fd = pipes[i][1];
        pids[i] = spawn_child(cmds[i], in_fd, out_fd, data);
        if (pids[i] == -1)
            return (-1);
        i++;
    }
    return (0);
}

static int  create_pipes(int (*pipes)[2], int n)
{
    int i;
    int j;

    i = 0;
    while (i <  n - 1)
    {
        while (pipe(pipes[i]) == -1)
        {
            if (errno == EINTR)
                continue;
            j = 0;
            while (j < i)
            {
                if (pipes[j][0] >= 0)
                    close(pipes[j][0]);
                if (pipes[j][1] >= 0)
                    close(pipes[j][1]);
                j++;
            }
            return (-1);
        }
        i++;
    }
    return (0);
}

static int allocate_resources(int n, int (*(*pipes_out))[2], pid_t **pids_out)
{
	int (*pipes)[2];
	pid_t *pids;
	int i;

	pipes = safe_malloc(sizeof(int[2]) * (n - 1));
	pids = safe_malloc(sizeof(pid_t) * n);
	i = 0;
	while (i < n - 1)
	{
		pipes[i][0] = -1;
		pipes[i][1] = -1;
		i++;
	}
	i = 0;
	while (i < n)
	{
		pids[i] = 0;
		i++;
	}
	*pipes_out = pipes;
	*pids_out = pids;
	return (0);
}

int run_multi(t_ast **cmds, int n, t_data *data)
{
	int (*pipes)[2];
	pid_t *pids;
	int status;

	if (n < 2)
		return (run_single(cmds[0], data));
	if (allocate_resources(n, &pipes, &pids) == -1)
        return (127);
	if (create_pipes(pipes, n) == -1)
	{
		close_all_pipes(pipes, n);
		free(pids);
		free(pipes);
		return (127);
	}
	if (spawn_children_checked(cmds, n, pipes, data, pids) == -1)
		return (127);
	parent_close_intermediates(pipes, n);
	forward_fd(pipes[n - 2][0]);
	pipes[n - 2][0] = -1; /* avoid double-close */
	status = wait_for_all(pids, n);
	free(pids);
	close_all_pipes(pipes, n);
	free(pipes);
	return (status);
}
