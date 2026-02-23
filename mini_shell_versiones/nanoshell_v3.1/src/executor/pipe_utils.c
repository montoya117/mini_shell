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

pid_t wait_one(pid_t pid, int *st)
{
	pid_t w;

	if (pid <= 0 || !st)
		return (-1);
	while ((w = waitpid(pid, st, 0)) == -1 && errno == EINTR)
		;
	return (w);
}

int status_to_code(int s)
{
	if (WIFEXITED(s))
		return (WEXITSTATUS(s));
	if (WIFSIGNALED(s))
		return (128 + WTERMSIG(s));
	return (s);
}

int spawn_children_checked(t_ast **cmds, int n, int (*pipes)[2],
				  t_data *data, pid_t *pids)
{
	if (spawn_all_children(cmds, n, pipes, data, pids) == -1)
	{
		wait_for_all(pids, n);
		close_all_pipes(pipes, n);
		free(pids);
		free(pipes);
		return (-1);
	}
	return (0);
}


