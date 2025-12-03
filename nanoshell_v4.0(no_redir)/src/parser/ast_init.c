
/*
// Main AST node for shell commands, pipes, redirects, logic, subshells
typedef struct s_ast {
    AST_TYPE type;          // What type of AST node this is

    // For binary nodes (AND, OR, REDIRECT, SUBSHELL, sometimes PIPE)
    struct s_ast *left;    // Left child:
                           //  - AST_AND / AST_OR: left side of logical op
                           //  - AST_REDIRECT: the command to redirect
                           //  - AST_PIPE (if binary): the first command
                           //  - AST_SUBSHELL: subtree inside the subshell
                           //  - NULL for AST_COMMAND, N-ary PIPE

    struct s_ast *right;   // Right child:
                           //  - AST_AND / AST_OR: right side of logical op
                           //  - AST_PIPE (if binary): the second command
                           //  - (Rarely used for redirect)
                           //  - NULL for AST_COMMAND, SUBSHELL, N-ary PIPE

    char **argv;           // Arguments for command:
                           //  - AST_COMMAND: array of command and its arguments
                           //  - NULL for other node types

    char *file;            // Filename for redirect:
                           //  - AST_REDIRECT: target file name for redirect (e.g., output.txt)
                           //  - NULL for other node types

    int redirect_type;     // Kind of redirect:
                           //  - AST_REDIRECT: (e.g., input, output, append)
                           //  - 0 or unused for other types

    t_ast_list *children;  // N-ary children:
                           //  - AST_PIPE: linked list of command nodes when more than two
                           //  - NULL for AST_COMMAND, REDIRECT, AND, OR, SUBSHELL
} t_ast;

*/

#include "nanoshell.h"

// Command node constructor
t_ast *ast_new_command(char **tokens)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->argv = tokens;
	node->file = NULL;
	node->redirect_type = 0;
	node->children = NULL;
	return (node);
}

// Pipe node constructor
t_ast *ast_new_pipe(t_ast_list *children)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_PIPE;
	node->left = NULL;
    node->right = NULL;
    node->argv = NULL;
    node->file = NULL;
    node->redirect_type = 0;
    node->children = children;
	return (node);
}

// Redirect node constructor
t_ast *ast_new_redirect(t_ast *cmd, char *file, int redirect_type)
{
	t_ast   *node;

    node = safe_malloc(sizeof(t_ast));
    node->type = AST_REDIRECT;
	node->left = cmd;
	node->right = NULL;
	node->argv = NULL;
	node->file = file;
	node->redirect_type = redirect_type;
	node->children = NULL;
	return (node);
}
// Logic AND/OR node constructor
t_ast *ast_new_and(t_ast *left, t_ast *right)
{
	t_ast   *node;

    node = safe_malloc(sizeof(t_ast));
    node->type = AST_AND;
    node->left = left;
    node->right = right;
    node->argv = NULL;
    node->file = NULL;
    node->redirect_type = 0;
    node->children = NULL;
    return (node);
}

t_ast *ast_new_or(t_ast *left, t_ast *right)
{
    t_ast   *node;

    node = safe_malloc(sizeof(t_ast));
    node->type = AST_OR;
    node->left = left;
    node->right = right;
    node->argv = NULL;
    node->file = NULL;
    node->redirect_type = 0;
    node->children = NULL;
    return (node);
}

// Subshell node constructor
t_ast *ast_new_subshell(t_ast *child)
{
    t_ast   *node;

    node = malloc(sizeof(t_ast));
    node->type = AST_SUBSHELL;
    node->left = child;
    node->right = NULL;
    node->argv = NULL;
    node->file = NULL;
    node->redirect_type = 0;
    node->children = NULL;
    return (node);
}
