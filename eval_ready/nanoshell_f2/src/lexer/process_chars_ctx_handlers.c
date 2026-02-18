/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_chars_ctx_handlers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:52:05 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:52:15 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

/*
int	dispatch_char(t_word_ctx *ctx, const char *line,
	size_t *i, size_t len, int last_status, t_data *data)
{
	char	c;

	c = line[*i];
	if (c == '\'')
		return (handle_single_quote(ctx, line, i, len));
	if (c == '"')
		return (handle_double_quote(ctx, line, i, len, last_status, data));
	if (c == '$')
		return (handle_dollar(ctx, line, i, len, last_status, data));
	return (handle_regular_char(ctx, line, i));
}
int	handle_backslash_outside(t_word_ctx *ctx,
	const char *line, size_t *i, size_t len)
{
	ctx->seen_unquoted = 1;
	(*i)++;
	if (*i < len)
	{
		if (buf_append_char(&ctx->buf, line[*i]) < 0)
			return (-3);
		(*i)++;
		return (0);
	}
	if (buf_append_char(&ctx->buf, '\\') < 0)
		return (-3);
	return (0);
}*/

int	handle_single_quote(t_word_ctx *ctx, const char *line,
	size_t *i, size_t len)
{
	if (!ctx || !line || !i)
		return (-1);
	ctx->seen_single = 1;
	ctx->was_quoted = 1;
	(*i)++;
	if (parse_single_quote(&ctx->buf, line, i, len) < 0)
		return (-1);
	return (0);
}
