
/*
Memory Management
-----------------------------------------
void ast_free(t_ast *node);             ✅
Recursively frees an entire AST and its children.

void ast_list_free(t_ast_list *list); ✅
Recursively frees an AST children list.

Error Handling and Recovery
------------------------------------------
void ast_parser_error(const char *msg, t_token *tok);
Standardizes parser error messages and possibly halts parsing.

*/

#include "nanoshell.h"

// Memory management & free
// Recursively frees an AST children list.
void ast_list_free(t_ast_list *list)
{
	if (!list)
		return ;
	ast_free(list->node);
	ast_list_free(list->next);
	free(list);
}

// free ast_nodes & children
void ast_free(t_ast *node)
{
    int i;

	if (!node)
		return ;
	ast_free(node->left);
	ast_free(node->right);
	ast_list_free(node->children);
	if (node->argv)
    {
        i = 0;
        while(node->argv[i])
        {
            free(node->argv[i]);
            i++;
        }
        free(node->argv);
    }
    if (node->assigments)
    {
        i = 0;
        while(node->assigments[i])
        {
            free(node->assigments[i]);
            i++;
        }
        free(node->assigments);
    }
    free(node->file);
	free(node);
}

// Error Handling

int ft_strlen_const(const char *str)
{
    int i;

    i = 0;
    while(str[i])
        i++;
    return (i);
}

void ast_parser_error(const char *message, t_token *token)
{
    const char  *prefix;
    const char  *errstr;
    const char  *suffix;

    prefix = "Parse error: ";
    errstr = NULL;
    // Print your base error message 
    write(2, prefix, ft_strlen_const(prefix));
    if (message)
        write (2, message, ft_strlen_const(message));
    // If there's a token, print info about it
    if (token)
    {
        write (2, " at token '", 11);
        if (token->text)
            write (2, token->text, ft_strlen_const(token->text));
        write (2, "'\n", 2);
    }
    else
    {
        suffix = " at the end of input\n";
        write(2, suffix, ft_strlen_const(suffix));
    }
	// If errno is set, print details
    if (errno != 0)
    {
        errstr = strerror(errno);
        write (2, "System error: ", 14);
        write(2, errstr, ft_strlen_const(errstr));
        write(2, "\n", 1);
    }
}
