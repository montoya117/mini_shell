/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:20:02 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:20:04 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

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
static void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static void	free_assignments(char **assignments)
{
	int	i;

	if (!assignments)
		return ;
	i = 0;
	while (assignments[i])
	{
		free(assignments[i]);
		i++;
	}
	free(assignments);
}

void	ast_free(t_ast *node)
{
	if (!node)
		return ;
	ast_free(node->left);
	ast_free(node->right);
	ast_list_free(node->children);
	free_argv(node->argv);
	free_assignments(node->assignments);
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
