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

static int	append_piece_char(t_word_ctx *ctx, char c, int quoted)
{
	char	tmp[2];

	tmp[0] = c;
	tmp[1] = '\0';
	if (ctx->buf.data && ctx->buf.len > 0 && ctx->buf_quoted != quoted)
	{
		if (flush_buf_as_piece(ctx) < 0)
			return (-1);
	}
	ctx->buf_quoted = quoted;
	return (buf_append_str(&ctx->buf, tmp));
}

int	handle_single_quote(t_word_ctx *ctx, const char *line, size_t *i, size_t len)
{
	if (!ctx || !line || !i)
		return (-1);
	ctx->seen_single = 1;

	if (flush_buf_as_piece(ctx) < 0)
		return (-1);
	ctx->buf_quoted = 1;

	(*i)++;
	while (*i < len && line[*i] != '\'')
	{
		if (append_piece_char(ctx, line[*i], 1) < 0)
			return (-1);
		(*i)++;
	}
	if (*i >= len)
		return (-1);
	(*i)++;

	if (flush_buf_as_piece(ctx) < 0)
		return (-1);
	ctx->buf_quoted = 0;
	return (0);
}