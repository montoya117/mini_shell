/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:02:42 by jadelgad          #+#    #+#             */
/*   Updated: 2026/01/14 12:10:48 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static const char	*type_to_str(t_token_type t)
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

static const char	*quote_to_str(t_quote_type q)
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

const char	*display_text_for_token(const t_token *t)
{
	if (t == NULL)
		return ("");
	if (t->text && t->text[0] != '\0')
		return (t->text);
	if (t->type == TOKEN_PIPE)
		return ("|");
	else if (t->type == TOKEN_REDIR_OUT)
		return (">");
	else if (t->type == TOKEN_REDIR_APPEND)
		return (">>");
	else if (t->type == TOKEN_REDIR_IN)
		return ("<");
	else if (t->type == TOKEN_HEREDOC)
		return ("<<");
	else if (t->type == TOKEN_AND)
		return ("&&");
	else if (t->type == TOKEN_OR)
		return ("||");
	else if (t->type == TOKEN_LPAREN)
		return ("(");
	else if (t->type == TOKEN_RPAREN)
		return (")");
	else if (t->type == TOKEN_EOF)
		return ("EOF");
	else if (t->type == TOKEN_ERROR)
		return ("ERROR");
	return ("");
}

static void	print_escaped(const char *s)
{
	unsigned char	c;

	if (!s)
	{
		printf("(null)");
		return ;
	}
	while (*s)
	{
		c = (unsigned char)*s;
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
		s++;
	}
}

void	tokens_print_simple(const t_token *head)
{
	t_token	*cur;
	int		idx;
	char	*raw;
	char	*display;

	cur = (t_token *)head;
	idx = 1;
	while (cur)
	{
		raw = cur->text;
		display = display_text_for_token(cur);
		printf("token%-3d type=%-12s pos=%4zu quote=%-6s raw=\"",
			idx, type_to_str(cur->type), cur->pos, quote_to_str(cur->quote));
		print_escaped(raw);
		printf("\" display=\"");
		if (display)
			printf("%s", display);
		else
			printf("");
		if (cur->type == TOKEN_ERROR)
			printf("  <-- ERROR");
		printf("\n");
		cur = cur->next;
		idx++;
	}
}

void	tokens_print_simple_array(const t_token **arr, size_t count)
{
	size_t			i;
	const t_token	*t;

	i = 0;
	while (i < count)
	{
		t = arr[i];
		if (!t)
		{
			printf("token");
			printf("%zu", i + 1);
			printf(": (NULL)\n");
		}
		else
		{
			printf("token");
			printf("%-3zu", i + 1);
			printf(" type=");
			printf("%-12s", type_to_str(t->type));
			printf(" pos=");
			printf("%4d", (int)t->pos);
			printf(" quote=");
			printf("%-6s", quote_to_str(t->quote));
			printf(" raw=\"");
			print_escaped(t->text);
			printf("\" display=\"");
			if (display_text_for_token(t))
				printf("%s", display_text_for_token(t));
			else
				printf("");
			printf("\"\n");
		}
		i++;
	}
}
