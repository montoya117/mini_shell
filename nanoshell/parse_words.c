/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 14:06:18 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:03:28 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static t_quote_type	quote_type_from_flags(int seen_single, int seen_double)
{
	if (seen_single && seen_double)
		return (QT_MIXED);
	if (seen_single)
		return (QT_SINGLE);
	if (seen_double)
		return (QT_DOUBLE);
	return (QT_NONE);
}

static t_token	*finalize_token_from_ctx(size_t start, t_word_ctx *ctx)
{
	char			*text;
	t_quote_type	qt;

	text = buf_release(&ctx->buf);
	if (!text)
		return (NULL);
	qt = quote_type_from_flags(ctx->seen_single, ctx->seen_double);
	return (token_new(TOKEN_WORD, text, qt, (int)start));
}

static int	process_chars_ctx(t_word_ctx *ctx, const char *line, size_t *i, size_t len)
{
	char	c;

	while (*i < len && !is_space((char)line[*i]) && !is_operator((char)line[*i]))
	{
		c = line[*i];
		if (c == '\'')
		{
			ctx->seen_single = 1;
			(*i)++;
			if (parse_single_quote(&ctx->buf, line, i, len) < 0)
				return (-1);
			continue ;
		}
		if (c == '"')
		{
			ctx->seen_double = 1;
			(*i)++;
			if (parse_double_quote(&ctx->buf, line, i , len) < 0)
				return (-2);
			continue ;
		}
		if (buf_append_char(&ctx->buf, c) < 0)
				return (-3);
		(*i)++;
	}
	return (0);
}

static t_token	*build_word_token(const char *line, size_t *i, size_t len, size_t start)
{
	t_word_ctx	ctx;
	int			rc;
	t_token		*tok;

	rc = 0;
	tok = NULL;
	ctx.seen_single = 0;
	ctx.seen_double = 0;

	buf_init(&ctx.buf);
	rc = process_chars_ctx(&ctx, line, i, len);
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

t_token	*parse_word(const char *line, size_t *i, size_t len)
{
	size_t	start;

	if (!line || !i)
		return (NULL);
	start = *i;
	return (build_word_token(line, i , len, start));
}
