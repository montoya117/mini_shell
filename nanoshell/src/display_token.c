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

static const char *type_to_str(t_token_type t)
{
    if (t == TOKEN_WORD) return ("WORD");
    if (t == TOKEN_PIPE) return ("PIPE");
    if (t == TOKEN_REDIR_IN) return ("REDIR_IN");
    if (t == TOKEN_REDIR_OUT) return ("REDIR_OUT");
    if (t == TOKEN_REDIR_APPEND) return ("REDIR_APPEND");
    if (t == TOKEN_HEREDOC) return ("HEREDOC");
    if (t == TOKEN_AND) return ("AND");
    if (t == TOKEN_OR) return ("OR");
    if (t == TOKEN_LPAREN) return ("LPAREN");
    if (t == TOKEN_RPAREN) return ("RPAREN");
    if (t == TOKEN_EOF) return ("EOF");
    if (t == TOKEN_ERROR) return ("ERROR");
    return ("UNKNOWN");
}


static const char *quote_to_str(t_quote_type q)
{
    if (q == QT_NONE)   return ("NONE");
    if (q == QT_SINGLE) return ("SINGLE");
    if (q == QT_DOUBLE) return ("DOUBLE");
    if (q == QT_MIXED)  return ("MIXED");
    return ("UNKNOWN");
}


const char *display_text_for_token(const t_token *t)
{
	if (t == NULL)
		return "";

	if (t->text && t->text[0] != '\0')
		return t->text;

	switch (t->type)
	{
		case TOKEN_PIPE: return "|";
		case TOKEN_REDIR_OUT: return ">";
		case TOKEN_REDIR_APPEND: return ">>";
		case TOKEN_REDIR_IN: return "<";
		case TOKEN_HEREDOC: return "<<";
		case TOKEN_AND: return "&&";
		case TOKEN_OR: return "||";
		case TOKEN_LPAREN: return "(";
		case TOKEN_RPAREN: return ")";
		case TOKEN_EOF: return "EOF";
		case TOKEN_ERROR: return "ERROR";
		case TOKEN_WORD:
		default:
			return "";
	}
}


static void print_escaped(const char *s)
{
	if (!s)
	{
		printf("(null)");
		return;
	}
	while (*s)
	{
		unsigned char c = (unsigned char)*s;
		switch (c)
		{
			case '\n': printf("\\n"); break;
			case '\r': printf("\\r"); break;
			case '\t': printf("\\t"); break;
			case '\b': printf("\\b"); break;
			case '\a': printf("\\a"); break;
			case '\v': printf("\\v"); break;
			case '\\': printf("\\\\"); break;
			case '\"': printf("\\\""); break;
			default:
				if (isprint(c))
					putchar(c);
				else
					printf("\\x%02x", c);
				break;
		}
		s++;
	}
}


void tokens_print_simple(const t_token *head)
{
	const t_token *cur = head;
	int idx = 1;

	while (cur)
	{
		const char *raw = cur->text;
		const char *display = display_text_for_token(cur);

		printf("token%-3d type=%-12s pos=%4zu quote=%-6s raw=\"",
			   idx, type_to_str(cur->type), cur->pos, quote_to_str(cur->quote));
		print_escaped(raw);
		printf("\" display=\"%s\"", display ? display : "");
		if (cur->type == TOKEN_ERROR)
			printf("  <-- ERROR");
		printf("\n");

		cur = cur->next;
		idx++;
	}
}


void tokens_print_simple_array(const t_token **arr, size_t count)
{
	size_t i;
	for (i = 0; i < count; ++i)
	{
		const t_token *t = arr[i];
		if (!t)
		{
			printf("token%zu: (NULL)\n", i + 1);
			continue;
		}
		printf("token%-3zu type=%-12s pos=%4d quote=%-6s raw=\"",
			   i + 1,
			   type_to_str(t->type),
			   (int)t->pos,
			   quote_to_str(t->quote));
		print_escaped(t->text);
		printf("\" display=\"%s\"\n", display_text_for_token(t));
	}
}
