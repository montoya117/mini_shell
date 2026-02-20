/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_chars_ctx.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:52:54 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:52:58 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

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

static int	handle_double_quote_ctx_word(t_word_ctx *ctx,
		t_quote_ctx *qctx, size_t *i)
{
	int	rc;

	if (!ctx || !qctx || !i)
		return (-2);
	ctx->seen_double = 1;

	(*i)++; /* skip " */
	if (flush_buf_as_piece(ctx) < 0)
		return (-2);
	ctx->buf_quoted = 1;

	rc = parse_double_quote(qctx, i);
	if (rc < 0)
		return (-2);

	if (flush_buf_as_piece(ctx) < 0)
		return (-2);
	ctx->buf_quoted = 0;
	return (0);
}

static int	handle_dollar_ctx_word(t_word_ctx *ctx, t_quote_ctx *qctx, size_t *i)
{
	(void)ctx;
	if (!qctx || !i)
		return (-4);
	if (expand_dollar_ctx(qctx, i) < 0)
		return (-4);
	return (0);
}

int	handle_regular_char(t_word_ctx *ctx, const char *line, size_t *i)
{
	char	tmp[2];

	if (!ctx || !line || !i)
		return (-3);
	ctx->seen_unquoted = 1;
	tmp[0] = line[*i];
	tmp[1] = '\0';
	ctx->buf_quoted = 0;
	if (buf_append_str(&ctx->buf, tmp) < 0)
		return (-3);
	(*i)++;
	return (0);
}

static int	dispatch_char_ctx(t_word_ctx *ctx, t_word_src *src, size_t *i)
{
	char		c;
	t_quote_ctx	qctx;

	c = src->line[*i];
	if (c == '\'')
		return (handle_single_quote(ctx, src->line, i, src->len));
	if (c == '"')
	{
		qctx.buf = &ctx->buf;
		qctx.line = src->line;
		qctx.len = src->len;
		qctx.last_status = src->last_status;
		qctx.data = src->data;
		qctx.wctx = ctx;
		qctx.in_dquote = 1;
		return (handle_double_quote_ctx_word(ctx, &qctx, i));
	}
	if (c == '$')
	{
		qctx.buf = &ctx->buf;
		qctx.line = src->line;
		qctx.len = src->len;
		qctx.last_status = src->last_status;
		qctx.data = src->data;
		qctx.wctx = ctx;
		qctx.in_dquote = 0;
		return (handle_dollar_ctx_word(ctx, &qctx, i));
	}
	return (handle_regular_char(ctx, src->line, i));
}

int	process_chars_ctx(t_word_ctx *ctx, t_word_src *src, size_t *i)
{
	int	rc;

	while (*i < src->len && !is_space((char)src->line[*i])
		&& !is_operator((char)src->line[*i]))
	{
		rc = dispatch_char_ctx(ctx, src, i);
		if (rc != 0)
			return (rc);
	}
	return (0);
}