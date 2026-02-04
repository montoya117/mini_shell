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

int	expand_special_status(t_buf *buf, size_t *i, int last_status)
{
	char	*s;
	int		rc;

	if (!buf || !i)
		return (-1);
	s = ft_itoa(last_status);
	if (!s)
		return (-1);
	rc = buf_append_str(buf, s);
	free(s);
	if (rc < 0)
		return (-1);
	(*i)++;
	return (0);
}

int	expand_special_pid(t_buf *buf, size_t *i)
{
	char	*s;
	int		rc;

	if (!buf || !i)
		return (-1);
	s = ft_itoa((int)getpid());
	if (!s)
		return (-1);
	rc = buf_append_str(buf, s);
	free(s);
	if (rc < 0)
		return (-1);
	(*i)++;
	return (0);
}

int	expand_dollar(t_expand_ctx *ctx, size_t *i)
{
	if (!ctx || !ctx->buf || !ctx->line || !i)
		return (-1);
	if (*i >= ctx->len || ctx->line[*i] != '$')
		return (0);
	(*i)++;
	if (*i >= ctx->len)
	{
		if (buf_append_char(ctx->buf, '$') < 0)
			return (-1);
		return (0);
	}
	if (ctx->line[*i] == '?')
		return (expand_special_status(ctx->buf, i, ctx->last_status));
	if (ctx->line[*i] == '$')
		return (expand_special_pid(ctx->buf, i));
	if (ctx->line[*i] == '{')
		return (handle_braced_ctx(ctx, i));
	return (handle_simple_ctx(ctx, i));
}

/* Wrapper con contexto para los llamadores existentes */
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
	return (expand_dollar(&ctx, i));
}
