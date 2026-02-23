
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

void	apply_redirect_and_exec_child(t_ast *node, t_data *data)
{
	int		fd;
	t_ast	*child;
	t_ast	**reds;
	int		n;
	int		i;

	if (!node || !node->left)
		exit (1);
	// 1 contar cuántos AST_REDIRECT hay
	n = 0;
	child = node;
	while ( child && child->type == AST_REDIRECT)
	{
		n++;
		child = child->left;
	}
	if (!child)
		exit (1);
	// 2 reservar array de punteros
	reds = safe_malloc(n * sizeof(t_ast *));
	// 3 rellenar array en orden de la cadena
	child = node;
	i = 0;
	while (child && child->type == AST_REDIRECT && i < n)
	{
		reds[i] = child;
		i++;
		child = child->left;
	}
	// 4 aplicar redirecciones en orden inverso
	i = n - 1;
	while (i >= 0) 
	{
		// > Open file for writing, truncating if it exists
		fd = open_redirect(reds[i]);
		if (fd < 0)
		{
			free(reds);
            exit(1);
		}
		if (dup2(fd, reds[i]->redirect_fd) < 0)
        {
            exec_error("dup2 failed", reds[i]->file);
            close(fd);
			free(reds);
            exit(1);
        }
		close(fd);
        i--;
	}
	free(reds);
	// 5 ejecutar comando / subárbol 
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
