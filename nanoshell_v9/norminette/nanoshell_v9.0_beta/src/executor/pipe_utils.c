/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:58:13 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:58:17 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

pid_t	wait_one(pid_t pid, int *st)
{
	pid_t	w;

	if (pid <= 0 || st == NULL)
		return (-1);
	w = waitpid(pid, st, 0);
	while (w == -1 && errno == EINTR)
	{
		w = waitpid(pid, st, 0);
	}
	return (w);
}

int	status_to_code(int s)
{
	if (WIFEXITED(s))
		return (WEXITSTATUS(s));
	if (WIFSIGNALED(s))
		return (128 + WTERMSIG(s));
	return (s);
}

int	spawn_children_checked(t_ast **cmds, int n, int (*pipes)[2],
	t_data *data, pid_t *pids)
{
	if (spawn_all_children(cmds, n, pipes, data, pids) == -1)
	{
		wait_for_all(pids, n);
		close_all_pipes(pipes, n);
		return (-1);
	}
	return (0);
}

int	setup_resources(int n, int (*(*pipes_out))[2], pid_t **pids_out, t_data *data)
{
	int		(*pipes)[2];
	pid_t	*pids;

	if (allocate_resources(n, &pipes, &pids) == -1)
		return (-1);
	if (data)
	{
		data->pipes = pipes;
		data->pipes_count = n;
	}
	if (create_pipes(pipes, n) == -1)
	{
		close_all_pipes(pipes, n);
		free(pids);
		free(pipes);
		if (data)
		{
			data->pipes = NULL;
			data->pipes_count = 0;
		}
		return (-1);
	}
	*pipes_out = pipes;
	*pids_out = pids;
	return (0);
}

void	teardown_resources(int (*pipes)[2], pid_t *pids, int n, t_data *data)
{
	if (pids)
		free(pids);
	if (pipes)
	{
		close_all_pipes(pipes, n);
		free(pipes);
	}
	if (data)
	{
		data->pipes = NULL;
		data->pipes_count = 0;
	}
}
