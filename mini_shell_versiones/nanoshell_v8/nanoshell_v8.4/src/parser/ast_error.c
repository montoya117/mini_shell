
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
    if (node->assignments)
    {
        i = 0;
        while(node->assignments[i])
        {
            free(node->assignments[i]);
            i++;
        }
        free(node->assignments);
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

    prefix = "Minishell: ";
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

*/

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


/* Memory management & free */
/* Recursively frees an AST children list. */
void	ast_list_free(t_ast_list *list)
{
	if (!list)
		return ;
	ast_free(list->node);
	ast_list_free(list->next);
	free(list);
}

/* free ast_nodes & children */
void	ast_free(t_ast *node)
{
	int	i;

	if (!node)
		return ;
	ast_free(node->left);
	ast_free(node->right);
	ast_list_free(node->children);
	if (node->argv)
	{
		i = 0;
		while (node->argv[i])
		{
			free(node->argv[i]);
			i++;
		}
		free(node->argv);
	}
	if (node->assignments)
	{
		i = 0;
		while (node->assignments[i])
		{
			free(node->assignments[i]);
			i++;
		}
		free(node->assignments);
	}
	free(node->file);
	free(node);
}

/* Error Handling */

int	ft_strlen_const(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

static int	is_token_text(const t_token *token)
{
	if (!token)
		return (0);
	if (token->text && token->text[0] != '\0')
		return (1);
	return (0);
}

static const char	*token_display_fallback(const t_token *token)
{
	if (!token)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_REDIR_OUT)
		return (">");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (token->type == TOKEN_REDIR_IN)
		return ("<");
	if (token->type == TOKEN_HEREDOC)
		return ("<<");
	if (token->type == TOKEN_AND)
		return ("&&");
	if (token->type == TOKEN_OR)
		return ("||");
	if (token->type == TOKEN_LPAREN)
		return ("(");
	if (token->type == TOKEN_RPAREN)
		return (")");
	return ("newline");
}

static const char	*token_text_or_newline(const t_token *token)
{
	if (!token || token->type == TOKEN_EOF)
		return ("newline");
	if (is_token_text(token))
		return (token->text);
	return (token_display_fallback(token));
}

/*
Formato estándar Minishell:
minishell: syntax error near unexpected token `...'\n
- Ignora el texto heredado en `message`.
- No imprime errno ni "System error" para errores de parsing.
- Usa `newline` al final de la entrada.
*/
void	ast_parser_error(const char *message, t_token *token)
{
	const char	*prefix;
	const char	*msg;
	const char	*end;
	const char	*tokstr;

	(void)message;
	prefix = "minishell: ";
	msg = "syntax error near unexpected token `";
	end = "'\n";
	tokstr = token_text_or_newline(token);
	write(2, prefix, ft_strlen_const(prefix));
	write(2, msg, ft_strlen_const(msg));
	write(2, tokstr, ft_strlen_const(tokstr));
	write(2, end, ft_strlen_const(end));
	errno = 0;
}