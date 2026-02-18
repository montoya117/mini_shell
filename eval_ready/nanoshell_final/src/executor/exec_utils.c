/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:18:42 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:18:44 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	exec_error(const char *message, const char *subject)
{
	const char	*prefix;
	const char	*err;

	prefix = "minishell: ";
	write(2, prefix, ft_strlen_const(prefix));
	if (subject)
		write(2, subject, ft_strlen_const(subject));
	if (message && ft_strncmp(message, "command not found", 18) == 0)
	{
		write(2, ": command not found\n", 21);
		return ;
	}
	write(2, ": ", 2);
	if (errno != 0)
	{
		err = strerror(errno);
		if (err)
			write(2, err, ft_strlen_const(err));
		write(2, "\n", 1);
		return ;
	}
	if (message)
		write(2, message, ft_strlen_const(message));
	write(2, "\n", 1);
}

char	*heredoc_tmp_name(void)
{
	static int	n;
	char		*num;
	char		*name;

	if (n < 0)
		n = 0;
	num = ft_itoa(n);
	if (!num)
		return (NULL);
	n++;
	name = ft_strjoin(".heredoc_", num);
	free(num);
	return (name);
}

static int	is_delim(char *line, char *delimiter)
{
	size_t	len;

	if (!line)
		return (0);
	len = ft_strlen(delimiter);
	if (ft_strlen(line) == len && ft_strncmp(line, delimiter, len) == 0)
		return (1);
	return (0);
}

/*
** Returns 0 on success (EOF or delimiter found)
** Returns -1 if interrupted by SIGINT
*/

static int	write_heredoc_loop(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT)
		{
			if (line)
				free(line);
			return (-1);
		}
		if (!line)
			break ;
		if (is_delim(line, delimiter))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	return (0);
}

char	*create_heredoc_tmp(char *delimiter)
{
	char	*path;
	int		fd;

	path = heredoc_tmp_name();
	if (!path)
		return (NULL);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		exec_error("open failed", path);
		free(path);
		return (NULL);
	}
	if (write_heredoc_loop(fd, delimiter) == -1)
	{
		close(fd);
		unlink(path);
		free(path);
		g_signal_received = 0;
		return (NULL);
	}
	close(fd);
	return (path);
}

/*
char	*create_heredoc_tmp(char *delimeter)
{
	char	*path;
	char	*line;
	int		fd;
	size_t	len_line;
	size_t	len_delim;

	path = heredoc_tmp_name();
	if (!path)
		return (NULL);
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		exec_error("open failed", path);
		free(path);
		return (NULL);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_signal_received == SIGINT)
			{
				close(fd);
				unlink(path);
				free(path);
				g_signal_received = 0;
				return (NULL);
			}
			break ;
		}
		len_line = ft_strlen(line);
		len_delim = ft_strlen(delimeter);
		if (len_line == len_delim && ft_strncmp(line, delimeter,
				len_delim) == 0)
		{
			free(line);
			break ;
		}
		write (fd, line, ft_strlen(line));
		write (fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (path);
}
*/
