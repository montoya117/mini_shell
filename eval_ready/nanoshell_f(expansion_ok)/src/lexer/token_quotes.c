/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:50:51 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 14:27:58 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	parse_double_quote(t_quote_ctx *ctx, size_t *i)
{
	if (!ctx || !ctx->wctx)
		return (-1);
	while (*i < ctx->len && ctx->line[*i] != '"')
	{
		if (ctx->line[*i] == '\\' && (*i + 1) < ctx->len)
		{
			(*i)++;
			ctx->wctx->buf_quoted = 1;
			if (buf_append_char(&ctx->wctx->buf, ctx->line[*i]) < 0)
				return (-1);
			(*i)++;
			continue ;
		}
		if (ctx->line[*i] == '$')
		{
			if (expand_dollar_ctx(ctx, i) < 0)
				return (-1);
			continue ;
		}
		ctx->wctx->buf_quoted = 1;
		if (buf_append_char(&ctx->wctx->buf, ctx->line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	if (*i >= ctx->len)
		return (-1);
	(*i)++;
	return (0);
}
