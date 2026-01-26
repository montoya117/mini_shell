#include "nanoshell.h"

// Command node constructor
t_ast	*ast_new_command(char **tokens, char **assignments)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->argv = tokens;
	node->assignments = assignments;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	return (node);
}

// Pipe node constructor
t_ast	*ast_new_pipe(t_ast_list *children)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_PIPE;
	node->left = NULL;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = children;
	return (node);
}

// Redirect node constructor
t_ast	*ast_new_redirect(t_ast *cmd, char *file, int redirect_type)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_REDIRECT;
	node->left = cmd;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = file;
	node->redirect_type = redirect_type;
	node->children = NULL;
	if (redirect_type == TOKEN_REDIR_IN || redirect_type == TOKEN_HEREDOC)
		node->redirect_fd = 0;
	else
		node->redirect_fd = 1;
	return (node);
}

// Logic AND/OR node constructor
t_ast	*ast_new_and(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_AND;
	node->left = left;
	node->right = right;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	return (node);
}

t_ast	*ast_new_or(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_OR;
	node->left = left;
	node->right = right;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	return (node);
}

// Subshell node constructor
t_ast	*ast_new_subshell(t_ast *child)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	node->type = AST_SUBSHELL;
	node->left = child;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	return (node);
}
