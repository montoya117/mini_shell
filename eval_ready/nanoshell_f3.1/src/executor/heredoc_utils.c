
#include "nanoshell.h"

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

int	write_heredoc_loop(int fd, char *delimiter, int quoted, t_data *data)
{
	char	*line;
    char    *final;
	int		status;

	status = 0;
	signal(SIGINT, signal_handler_heredoc);
	while (1)
	{
		line = readline("> ");
		if (g_signal_received == SIGINT || !line || is_delim(line, delimiter))
		{
			if (g_signal_received == SIGINT)
				status = -1;
			break ;
		}
        // Si NO está entrecomillado, expandimos
		if (!quoted)
			final = expand_heredoc_line(line, data);
		else
			final = ft_strdup(line);
		write(fd, final, ft_strlen(final));
		write(fd, "\n", 1);
		free(line);
        free(final);
		line = NULL;
	}
	if (line)
		free(line);
	setup_signals();
	return (status);
}

char	*create_heredoc_tmp(char *delimiter, int quoted, t_data *data)
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
	if (write_heredoc_loop(fd, delimiter, quoted, data) == -1)
	{
		close(fd);
		unlink(path);
		free(path);
		return (NULL);
	}
	close(fd);
	return (path);
}
