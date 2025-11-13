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

const char *display_text_for_token(const t_token *t)
{
	if (t == NULL)
		return "";

	if (t->text && t->text[0] != '\0')
		return t->text;

	/* token->text es NULL o vacío: mostrar símbolo según tipo */
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
			/* fallback (debería haberse cubierto con text) */
			return "";
	}
}

/* Imprime tokens de una lista enlazada: token1 [texto] ... */
void tokens_print_simple(const t_token *head)
{
	const t_token *cur = head;
	int idx = 1;

	while (cur)
	{
		printf("token%d [%s] type=%s pos=%zu\n", idx, display_text_for_token(cur), type_to_str(cur->type), cur->pos);
		cur = cur->next;
		idx++;
	}
}

/* Variente para array de punteros: tokens_print_simple_array(arr, count) */
void tokens_print_simple_array(const t_token **arr, size_t count)
{
	size_t i;
	for (i = 0; i < count; ++i)
	{
		const t_token *t = arr[i];
		const char *txt = display_text_for_token(t);
		printf("token%zu [%s]\n", i + 1, txt);
	}
}

