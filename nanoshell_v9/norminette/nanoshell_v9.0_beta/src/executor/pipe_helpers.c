/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:56:48 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:56:55 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	close_all_pipes(int (*pipes)[2], int n)
{
	int	i;

	i = 0;
	if (!pipes || n <= 0)
	{
		return ;
	}
	while (i < n - 1)
	{
		if (pipes[i][0] >= 0)
		{
			close(pipes[i][0]);
			pipes[i][0] = -1;
		}
		if (pipes[i][1] >= 0)
		{
			close(pipes[i][1]);
			pipes[i][1] = -1;
		}
		i++;
	}
}

int	wait_for_all(pid_t *pids, int count)
{
	int		i;
	int		last;
	int		status;
	pid_t	w;

	if (!pids || count <= 0)
		return (0);
	i = 0;
	last = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			w = wait_one(pids[i], &status);
			if (w == -1)
				last = 127;
			else
				last = status_to_code(status);
		}
		i++;
	}
	return (last);
}
/*
static ssize_t write_all(int fd, const void *buf, size_t count)
{
	const char *p;
	ssize_t     total;
	ssize_t     w;

	p = (const char *)buf;
	total = 0;
	while ((size_t)total < count)
	{
		w = write(fd, p + (size_t)total, count - (size_t)total);
		if (w == 0)
			return (-1);
		if (w == -1)
		{
			if (errno == EINTR)
				continue;
			return (-1);
		}
	total += w;
	}
	return (total);
}
static ssize_t  safe_read(int fd, void *buf, size_t count)
{
	ssize_t r;

	while (1)
	{
		r = read(fd, buf, count);
		if (r == -1)
		{
			if (errno == EINTR)
				continue;
			return (-1);
		}
		return (r);
	}
}


int forward_fd(int fd)   ESTO ERA UTIL PARA RUN_SINGLE, NOW IS TRASH
{
	char    buf[4096];
	ssize_t r;

	if (fd < 0)
		return (-1);
	while ((r = safe_read(fd, buf, sizeof(buf))) > 0)
	{
		if (write_all(STDOUT_FILENO, buf, (size_t)r) == -1)
		{
			close(fd);
			return (-1);
		}
	}
	if (r == -1)
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}
*/
