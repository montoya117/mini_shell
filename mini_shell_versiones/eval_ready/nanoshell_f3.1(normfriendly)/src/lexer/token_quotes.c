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

int	parse_single_quote(t_buf *buf, const char *line, size_t *i, size_t len)
{
	while (*i < len && line[*i] != '\'')
	{
		if (buf_append_char(buf, line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	if (*i >= len)
		return (-1);
	(*i)++;
	return (0);
}

int	parse_double_quote(t_quote_ctx *ctx, size_t *i)
{
	while (*i < ctx->len && ctx->line[*i] != '"')
	{
		if (ctx->line[*i] == '\\' && (*i + 1) < ctx->len)
		{
			(*i)++;
			if (buf_append_char(ctx->buf, ctx->line[*i]) < 0)
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
		if (buf_append_char(ctx->buf, ctx->line[*i]) < 0)
			return (-1);
		(*i)++;
	}
	if (*i >= ctx->len)
		return (-1);
	(*i)++;
	return (0);
}
