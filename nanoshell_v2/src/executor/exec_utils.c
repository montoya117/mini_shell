
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
