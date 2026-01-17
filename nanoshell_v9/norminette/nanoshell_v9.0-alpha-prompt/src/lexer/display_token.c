/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:02:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:15:50 by jadelgad         ###   ########.fr       */
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

static const char	*display_text_for_type(int type)
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

static void	print_escaped_char(unsigned char c)
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

static void	print_escaped_loop(const char *s)
{
	unsigned char	c;

	while (*s)
	{
		c = (unsigned char)*s;
		print_escaped_char(c);
		s++;
	}
}

static void	print_escaped(const char *s)
{
	if (!s)
	{
		printf("(null)");
		return ;
	}
	print_escaped_loop(s);
}

void	tokens_print_simple(const t_token *head)
{
	t_token		*cur;
	int			idx;
	const char	*raw;
	const char	*display;

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
		printf("%s", display);
		if (cur->type == TOKEN_ERROR)
			printf("  <-- ERROR");
		printf("\n");
		cur = cur->next;
		idx++;
	}
}

static void	print_null_token(size_t i)
{
	printf("token");
	printf("%zu", i + 1);
	printf(": (NULL)\n");
}

static void	print_token_header(const t_token *t, size_t i)
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
}

static void	print_token_display_and_close(const t_token *t)
{
	const char	*display;

	printf("\" display=\"");
	display = display_text_for_token(t);
	if (display)
		printf("%s", display);
	printf("\"\n");
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
			print_null_token(i);
		}
		else
		{
			print_token_header(t, i);
			print_escaped(t->text);
			print_token_display_and_close(t);
		}
		i++;
	}
}
