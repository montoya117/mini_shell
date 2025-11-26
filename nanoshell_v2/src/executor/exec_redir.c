
#include "nanoshell.h"

int	apply_redirect_and_exec(t_ast *node, t_data *data)
{
	int		fd;
	pid_t	pid;
	int		status;

	if (!node || !node->left)
		return (1);
	pid = fork();
	if (pid < 0)
	{
		// fork error
		exec_error("fork failed", NULL);
		return (1);
	}
	else if (pid == 0)
	{
		// CHILD: set up redirections, then execute node->left
		/* set fd depending on redirect_type */
		fd = -1;
		if (node->type == TOKEN_REDIR_OUT)
		{
			// >
		}
		else if (node->type == TOKEN_REDIR_IN)
        {
            // >
        }
	}

}
