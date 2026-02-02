/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_token_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:02:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:15:50 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

const char	*type_to_str(t_token_type t)
{
	if (t == TOKEN_WORD)
		return ("WORD");
	if (t == TOKEN_PIPE)
		return ("PIPE");
	if (t == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (t == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (t == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (t == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (t == TOKEN_AND)
		return ("AND");
	if (t == TOKEN_OR)
		return ("OR");
	if (t == TOKEN_LPAREN)
		return ("LPAREN");
	if (t == TOKEN_RPAREN)
		return ("RPAREN");
	if (t == TOKEN_EOF)
		return ("EOF");
	if (t == TOKEN_ERROR)
		return ("ERROR");
	return ("UNKNOWN");
}

const char	*quote_to_str(t_quote_type q)
{
	if (q == QT_NONE)
		return ("NONE");
	if (q == QT_SINGLE)
		return ("SINGLE");
	if (q == QT_DOUBLE)
		return ("DOUBLE");
	if (q == QT_MIXED)
		return ("MIXED");
	return ("UNKNOWN");
}

const char	*display_text_for_type(int type)
{
	if (type == TOKEN_PIPE)
		return ("|");
	else if (type == TOKEN_REDIR_OUT)
		return (">");
	else if (type == TOKEN_REDIR_APPEND)
		return (">>");
	else if (type == TOKEN_REDIR_IN)
		return ("<");
	else if (type == TOKEN_HEREDOC)
		return ("<<");
	else if (type == TOKEN_AND)
		return ("&&");
	else if (type == TOKEN_OR)
		return ("||");
	else if (type == TOKEN_LPAREN)
		return ("(");
	else if (type == TOKEN_RPAREN)
		return (")");
	else if (type == TOKEN_EOF)
		return ("EOF");
	else if (type == TOKEN_ERROR)
		return ("ERROR");
	return ("");
}

const char	*display_text_for_token(const t_token *t)
{
	if (t == NULL)
		return ("");
	if (t->text && t->text[0] != '\0')
		return (t->text);
	return (display_text_for_type(t->type));
}

void	print_escaped_char(unsigned char c)
{
	if (c == '\n')
		printf("\\n");
	else if (c == '\r')
		printf("\\r");
	else if (c == '\t')
		printf("\\t");
	else if (c == '\b')
		printf("\\b");
	else if (c == '\a')
		printf("\\a");
	else if (c == '\v')
		printf("\\v");
	else if (c == '\\')
		printf("\\\\");
	else if (c == '\"')
		printf("\\\"");
	else if (isprint(c))
		putchar(c);
	else
		printf("\\x%02x", c);
}
