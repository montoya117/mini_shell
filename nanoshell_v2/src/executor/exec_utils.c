
#include "nanoshell.h"

void	exec_error(const char *message, const char *arg)
{
	write(2, "Executor: ", 10);
	write(2, message, ft_strlen_const(message));
	if (arg)
	{
		write(2, ":", 1);
		write(2, arg, ft_strlen_const(arg));
	}
	write(2, "\n", 1);
}

char *heredoc_tmp_name(void)
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

char *create_heredoc_tmp(char *delimeter)
{
	char	*path;
	char	*line;
	int		fd;
	size_t	len_line;
	size_t	len_delim;

	path = heredoc_tmp_name();
	if (!path)
		return (NULL);
	// only your minishell user can read/write the file
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
			break; // EOF
		len_line = ft_strlen(line);
		len_delim = ft_strlen(delimeter);
		if (len_line == len_delim && ft_strncmp(line, delimeter, len_delim) == 0)
		{
			free(line);
			break; // reached delimeter
		}
		write (fd, line, ft_strlen(line));
		write (fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (path); 
}
