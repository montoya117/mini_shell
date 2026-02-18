/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:06:18 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:03:28 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static t_quote_type	quote_type_from_flags(int seen_single, int seen_double,
	int seen_unquoted)
{
	if ((seen_single && seen_double)
		|| (seen_single && seen_unquoted) || (seen_double && seen_unquoted))
		return (QT_MIXED);
	if (seen_single)
		return (QT_SINGLE);
	if (seen_double)
		return (QT_DOUBLE);
	if (seen_unquoted)
		return (QT_NONE);
	return (QT_NONE);
}

static t_token	*finalize_token_from_ctx(size_t start, t_word_ctx *ctx)
{
	char			*text;
	t_quote_type	qt;
	t_token			*tok;

	text = buf_release(&ctx->buf);
	if (!text)
		return (NULL);
	qt = quote_type_from_flags(ctx->seen_single, ctx->seen_double,
			ctx->seen_unquoted);
	tok = token_new(TOKEN_WORD, text, qt, (int)start);
	if (!tok)
		return (NULL);
	tok->was_quoted = ctx->was_quoted;
	return (tok);
}
// is_io_number y join_next ya están a 0 por defecto

static t_token	*build_word_token_ctx(t_word_src *src, size_t *i, size_t start)
{
	t_word_ctx	ctx;
	int			rc;
	t_token		*tok;

	ctx.seen_single = 0;
	ctx.seen_double = 0;
	ctx.seen_unquoted = 0;
	ctx.was_quoted = 0;
	buf_init(&ctx.buf);
	rc = process_chars_ctx(&ctx, src, i);
	if (rc != 0)
	{
		if (rc == -1)
			return (make_error_token_from_ctx(start,
					"unclosed single quote", &ctx));
		if (rc == -2)
			return (make_error_token_from_ctx(start,
					"unclosed double quote", &ctx));
		free(ctx.buf.data);
		return (NULL);
	}
	tok = finalize_token_from_ctx(start, &ctx);
	return (tok);
}

/* <= 4 args: src + i */
t_token	*parse_word_ctx(t_word_src *src, size_t *i)
{
	size_t	start;

	if (!src || !src->line || !i)
		return (NULL);
	start = *i;
	return (build_word_token_ctx(src, i, start));
}
