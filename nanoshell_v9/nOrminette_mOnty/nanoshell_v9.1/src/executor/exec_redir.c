
#include "nanoshell.h"

static int	open_redirect(t_ast *redir)
{
	int	fd;

	fd = -1;
	if (redir->redirect_type == TOKEN_REDIR_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->redirect_type == TOKEN_REDIR_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->redirect_type == TOKEN_REDIR_IN
		|| redir->redirect_type == TOKEN_HEREDOC)
		fd = open(redir->file, O_RDONLY);
	if (fd < 0)
		exec_error("open failed", redir->file);
	return (fd);
}

static t_ast	*collect_redirs(t_ast *node, t_ast ***reds, int *n)
{
	t_ast	*tmp;
	int		i;

	tmp = node;
	*n = 0;
	while (tmp && tmp->type == AST_REDIRECT)
	{
		(*n)++;
		tmp = tmp->left;
	}
	if (*n == 0)
		return (tmp);
	*reds = safe_malloc((*n) * sizeof(t_ast *));
	tmp = node;
	i = 0;
	while (i < *n)
	{
		(*reds)[i++] = tmp;
		tmp = tmp->left;
	}
	return (tmp);
}

static void	apply_redirs(t_ast **reds, int n)
{
	int	fd;
	int	i;

	i = n - 1;
	while (i >= 0)
	{
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
		if (fd != reds[i]->redirect_fd)
			close(fd);
		i--;
	}
	free(reds);
}

// loop backwards for multiple redirs

void	apply_redirect_and_exec_child(t_ast *node, t_data *data)
{
	t_ast	*child;
	t_ast	**reds;
	int		n;

	if (!node)
		exit (1);
	n = 0;
	reds = NULL;
	child = collect_redirs(node, &reds, &n);
	if (reds)
		apply_redirs(reds, n);
	if (!child)
		exit(0);
	if (child->type == AST_COMMAND)
		exec_child(child, data);
	else
		exec_ast_in_child(child, data);
	exit(127);
}

int	apply_redirect_and_exec(t_ast *node, t_data *data)
{
	pid_t	pid;
	int		ret;
	t_ast	*tmp;

	if (!node)
		return (1);
	pid = fork();
	if (pid < 0)
	{
		exec_error("fork failed", NULL);
		return (1);
	}
	else if (pid == 0)
	{
		apply_redirect_and_exec_child(node, data);
		exit(1);
	}
	else
	{
		ret = wait_for_child(pid);
		tmp = node;
		while (tmp && tmp->type == AST_REDIRECT)
		{
			if (tmp->redirect_type == TOKEN_HEREDOC && tmp->file)
				unlink(tmp->file);
			tmp = tmp->left;
		}
		data->last_status = ret;
		return (data->last_status);
	}
}
