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

static int	flush_buf_as_piece(t_word_ctx *ctx)
{
	char	*txt;

	if (!ctx)
		return (-1);
	if (!ctx->buf.data || ctx->buf.len == 0)
		return (0);
	txt = buf_release(&ctx->buf);
	if (!txt)
		return (-1);
	piece_append(&ctx->pieces, piece_new(txt, ctx->buf_quoted));
	return (0);
}

static char	*join_pieces_text(t_piece *p)
{
	t_buf	b;

	buf_init(&b);
	while (p)
	{
		if (p->text && buf_append_str(&b, p->text) < 0)
		{
			buf_free(&b);
			return (NULL);
		}
		p = p->next;
	}
	return (buf_release(&b));
}

static t_token	*finalize_token_from_ctx(size_t start, t_word_ctx *ctx)
{
	t_token			*tok;
	t_quote_type	qt;

	if (flush_buf_as_piece(ctx) < 0)
		return (NULL);
	qt = quote_type_from_flags(ctx->seen_single, ctx->seen_double,
			ctx->seen_unquoted);

	tok = token_new(TOKEN_WORD, NULL, qt, (int)start);
	if (!tok)
		return (NULL);
	tok->pieces = ctx->pieces;
	ctx->pieces = NULL;

	/* debug text */
	tok->text = join_pieces_text(tok->pieces);
	if (!tok->text)
		tok->text = ft_strdup("");
	return (tok);
}

static t_token	*build_word_token_ctx(t_word_src *src, size_t *i, size_t start)
{
	t_word_ctx	ctx;
	int			rc;
	t_token		*tok;

	ft_memset(&ctx, 0, sizeof(ctx));
	ctx.buf_quoted = 0;
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
		if (ctx.pieces)
			piece_free_all(ctx.pieces);
		return (NULL);
	}
	tok = finalize_token_from_ctx(start, &ctx);
	return (tok);
}

t_token	*parse_word_ctx(t_word_src *src, size_t *i)
{
	size_t	start;

	if (!src || !src->line || !i)
		return (NULL);
	start = *i;
	return (build_word_token_ctx(src, i, start));
}