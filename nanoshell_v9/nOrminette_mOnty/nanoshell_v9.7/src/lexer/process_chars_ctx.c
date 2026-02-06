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

/* <= 4 args: usa contexto de comillas y el índice */
static int	handle_double_quote_ctx_word(t_word_ctx *ctx,
		t_quote_ctx *qctx, size_t *i)
{
	int	rc;

	if (!ctx || !qctx || !i)
		return (-2);
	ctx->seen_double = 1;
	ctx->was_quoted = 1;
	(*i)++;
	rc = parse_double_quote(qctx, i);
	if (rc < 0)
		return (-2);
	return (0);
}

/* <= 4 args: usa contexto de comillas y el índice */
static int	handle_dollar_ctx_word(t_word_ctx *ctx,
	t_quote_ctx *qctx, size_t *i)
{
	if (!ctx || !qctx || !i)
		return (-4);
	if (expand_dollar_ctx(qctx, i) < 0)
		return (-4);
	return (0);
}

/* <= 4 args */
int	handle_regular_char(t_word_ctx *ctx, const char *line, size_t *i)
{
	if (!ctx || !line || !i)
		return (-3);
	ctx->seen_unquoted = 1;
	if (buf_append_char(&ctx->buf, line[*i]) < 0)
		return (-3);
	(*i)++;
	return (0);
}

/* <= 4 args: ctx + src + i */
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
		return (handle_double_quote_ctx_word(ctx, &qctx, i));
	}
	if (c == '$')
	{
		qctx.buf = &ctx->buf;
		qctx.line = src->line;
		qctx.len = src->len;
		qctx.last_status = src->last_status;
		qctx.data = src->data;
		return (handle_dollar_ctx_word(ctx, &qctx, i));
	}
	return (handle_regular_char(ctx, src->line, i));
}

/* <= 4 args: ctx + src + i */
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

/*
void dbg_buf_print(const t_word_ctx *ctx)
{
	const char *d = ctx->buf.data ? ctx->buf.data : "(null)";
	fprintf(stderr, "[DBG] buf=\"%s\" seen_s=%d seen_d=%d seen_u=%d\n",
			d, ctx->seen_single, ctx->seen_double, ctx->seen_unquoted);
}

 Loop principal con trazas
int process_chars_ctx(t_word_ctx *ctx, const char *line,
							 size_t *i, size_t len, int last_status)
{
	int rc;
	fprintf(stderr, "[DBG] process_chars_ctx start i=%zu len=%zu\n", *i, len);
	while (*i < len && !is_space((char)line[*i]) &&
		   !is_operator((char)line[*i]))
	{
		rc = dispatch_char(ctx, line, i, len, last_status);
		if (rc != 0)
		{
			fprintf(stderr,
				"[DBG] process_chars_ctx returning rc=%d i=%zu\n", rc, *i);
			return (rc);
		}
	}
	fprintf(stderr, "[DBG] process_chars_ctx done i=%zu\n", *i);
	return (0);
}

*/