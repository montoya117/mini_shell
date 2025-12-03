
#include "nanoshell.h"

int	apply_redirect_and_exec_child(t_ast *node, t_data *data)
{
	int		fd;

	if (!node || !node->left)
		return (1);
	fd = -1; // default value
	if (node->redirect_type == TOKEN_REDIR_OUT)
	{
		// > Open file for writing, truncating if it exists
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			exec_error("open failed", node->file);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			exec_error("dup2 failed", node->file);
			close(fd);
			return (1);
		}
	}
	else if (node->redirect_type == TOKEN_REDIR_APPEND)
    {
    	// >> Opens file for writing, and if it exists, appends to the end
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			exec_error("open failed", node->file);
			return (1);
		}
		if (dup2(fd, STDOUT_FILENO) < 0)
		{
			exec_error("dup2 failed", node->file);
			close(fd);
			return (1);
		}		
	}
	else if (node->redirect_type == TOKEN_REDIR_IN)
    {
		// < Open file for reading
		fd = open(node->file, O_RDONLY);
		if (fd < 0)
		{
			exec_error("open failed", node->file);
			return (1);
		}
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			exec_error("dup2 failed", node->file);
			close(fd);
			return (1);
		}
	}
	else if (node->redirect_type == TOKEN_HEREDOC)
    {
		// << Prepare a temporary file or pipe
		// write heredoc content into node->file
		fd = open(node->file, O_RDONLY);
		if (fd < 0)
		{
			exec_error("open failed", node->file);
			return (1);
		}
		// The command will read its input from your heredoc, not from the terminal
		if (dup2(fd, STDIN_FILENO) < 0)
		{
			exec_error("dup2 failed", node->file);
			close(fd);
			return (1);
		}
	}
	/* only close if it was actually opened */
	if (fd >= 0)
		close(fd);
	int st = exec_ast_in_child(node->left, data);

	fprintf(stderr, "DEBUG exec_ast_in_child returned %d\n", st);

	return st;
	// now run the node->left command/subtree
	//return (exec_ast_in_child(node->left, data));
}	

int	apply_redirect_and_exec(t_ast *node, t_data *data)
{
	pid_t	pid;
	int		status;
	int		ret;

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
		
		status = apply_redirect_and_exec_child(node, data);
		exit(status); // hijo: salir con el status devuelto
	}
	else
	{
		// PARENT: wait for child and return its status
		ret = wait_for_child(pid);
		if (node->redirect_type == TOKEN_HEREDOC && node->file)
			unlink(node->file); // delete tmp file
		data->last_status = ret;
    	return (data->last_status);
	}
}	
