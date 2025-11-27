
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

char *create_heredoc_tmp(char *delimeter)
{
	/*
	// Pick a temp path (e.g. /tmp/nanoshell_hd_<pid>_unter>).

	// fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);

	// Loop:
	// line = readline("> ");

	// if line == NULL or strcmp(line, delim) == 0 → stop.

	// write line, then \n, to fd.

	// free line.

	// Close fd.

	// return (ft_strdup(path));
	*/
}