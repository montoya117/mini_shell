
#include "nanoshell.h"

static int open_redirect(t_ast *redir)
{
    int fd = -1; // default value

    if (redir->redirect_type == TOKEN_REDIR_OUT)
        fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redir->redirect_type == TOKEN_REDIR_APPEND)
        fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (redir->redirect_type == TOKEN_REDIR_IN
          || redir->redirect_type == TOKEN_HEREDOC)
        fd = open(redir->file, O_RDONLY);

    if (fd < 0)
        exec_error("open failed", redir->file);
    return fd;
}

static int get_target_fd(t_token_type type)
{
    if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
        return STDIN_FILENO;
    return STDOUT_FILENO;  // OUT y APPEND
}
void	apply_redirect_and_exec_child(t_ast *node, t_data *data)
{
	int		fd;
	t_ast	*child;

	if (!node || !node->left)
		exit (1);
	// 1. PRIMERO: aplicar TODAS las redirecciones del árbol
	child = node;
	while (child && child->type == AST_REDIRECT)
	{
		// > Open file for writing, truncating if it exists
		fd = open_redirect(child);
		if (fd < 0)
            exit(1);
		if (dup2(fd, get_target_fd(child->redirect_type)) < 0)
        {
            exec_error("dup2 failed", child->file);
            close(fd);
            exit(1);
        }
		close(fd);
        child = child->left;
	}
	if (!child)
        exit(1);

	if (child->type == AST_COMMAND)
        exec_child(child, data);          // exits on success/failure
    else
		exec_ast_in_child(child, data);   // must exit or exec internally
	
	exit(127); // fallback if those ever return
}	

int	apply_redirect_and_exec(t_ast *node, t_data *data)
{
	pid_t	pid;
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
		
		apply_redirect_and_exec_child(node, data); // never returns
		exit(1); // // unreachable but for safety
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
