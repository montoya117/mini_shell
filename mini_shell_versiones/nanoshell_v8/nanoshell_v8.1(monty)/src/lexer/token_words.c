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

static t_quote_type quote_type_from_flags(int seen_single, int seen_double, int seen_unquoted)
{
	if ((seen_single && seen_double) ||
	    (seen_single && seen_unquoted) ||
	    (seen_double && seen_unquoted))
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
	qt = quote_type_from_flags(ctx->seen_single, ctx->seen_double, ctx->seen_unquoted);
	tok = token_new(TOKEN_WORD, text, qt, (int)start);
	if (!tok)
		return (NULL);
	tok->was_quoted = ctx->was_quoted;
	// is_io_number y join_next ya están a 0 por defecto
	return (tok);
}

static t_token *build_word_token(const char *line, size_t *i, size_t len, size_t start, int last_status, t_data *data)
{
    t_word_ctx ctx;
    int rc;
    t_token *tok;

    ctx.seen_single = 0;
    ctx.seen_double = 0;
    ctx.seen_unquoted = 0;
	ctx.was_quoted = 0; // new!!
    buf_init(&ctx.buf);

    rc = process_chars_ctx(&ctx, line, i, len, last_status, data);
    if (rc != 0)
    {
        if (rc == -1)
            return (make_error_token_from_ctx(start, "unclosed single quote", &ctx));
        if (rc == -2)
            return (make_error_token_from_ctx(start, "unclosed double quote", &ctx));
        free(ctx.buf.data);
        return (NULL);
    }
    tok = finalize_token_from_ctx(start, &ctx);
    return (tok);
}

t_token	*parse_word(const char *line, size_t *i, size_t len, int last_status, t_data *data)
{
	size_t	start;

	if (!line || !i)
		return (NULL);
	start = *i;
	return (build_word_token(line, i , len, start, last_status, data));
}
