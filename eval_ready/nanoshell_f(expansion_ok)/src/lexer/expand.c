/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:50:38 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:50:47 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "nanoshell.h"

static int	append_text_expanded(t_expand_ctx *ctx, const char *s)
{
	if (ctx && ctx->wctx)
	{
		ctx->wctx->buf_quoted = ctx->quoted;
		return (buf_append_str(&ctx->wctx->buf, s));
	}
	return (buf_append_str(ctx->buf, s));
}

int	expand_dollar(t_expand_ctx *ctx, size_t *i)
{
	if (!ctx || !ctx->line || !i)
		return (-1);
	if (*i >= ctx->len || ctx->line[*i] != '$')
		return (0);
	(*i)++;
	if (*i >= ctx->len)
		return (append_text_expanded(ctx, "$") < 0 ? -1 : 0);
	if (ctx->line[*i] == '?')
	{
		char	*s = ft_itoa(ctx->last_status);

		if (!s)
			return (-1);
		if (append_text_expanded(ctx, s) < 0)
			return (free(s), -1);
		free(s);
		(*i)++;
		return (0);
	}
	if (ctx->line[*i] == '$')
	{
		char	*s = ft_itoa((int)getpid());

		if (!s)
			return (-1);
		if (append_text_expanded(ctx, s) < 0)
			return (free(s), -1);
		free(s);
		(*i)++;
		return (0);
	}
	if (ctx->line[*i] == '{')
		return (handle_braced_ctx(ctx, i));
	return (handle_simple_ctx(ctx, i));
}

int	expand_dollar_ctx(t_quote_ctx *qctx, size_t *i)
{
	t_expand_ctx	ctx;

	if (!qctx || !i)
		return (-1);
	ctx.buf = qctx->buf;
	ctx.line = qctx->line;
	ctx.len = qctx->len;
	ctx.last_status = qctx->last_status;
	ctx.data = qctx->data;
	ctx.wctx = qctx->wctx;
	ctx.quoted = qctx->in_dquote;
	return (expand_dollar(&ctx, i));
}