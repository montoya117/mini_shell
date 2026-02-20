/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:51:07 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:51:12 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int	append_env_by_name(t_expand_ctx *ctx, const char *name)
{
	const char	*val;

	if (!ctx || !name || !ctx->data)
		return (0);
	val = get_var_from_envp(ctx->data->envp, name);
	if (!val)
		return (0);
	if (ctx->wctx)
		ctx->wctx->buf_quoted = ctx->quoted;
	if (ctx->wctx)
		return (buf_append_str(&ctx->wctx->buf, val));
	return (buf_append_str(ctx->buf, val));
}

static int	append_literal_range(t_buf *buf, const char *line,
		size_t start, size_t end)
{
	while (start < end)
	{
		if (buf_append_char(buf, line[start]) < 0)
			return (-1);
		start++;
	}
	return (0);
}

static int	braced_unclosed_append(t_expand_ctx *ctx,
		const char *line, size_t start, size_t current)
{
	t_buf	*buf;

	if (!ctx || !line)
		return (-1);
	buf = ctx->wctx ? &ctx->wctx->buf : ctx->buf;
	if (ctx->wctx)
		ctx->wctx->buf_quoted = ctx->quoted;
	if (buf_append_char(buf, '$') < 0)
		return (-1);
	if (buf_append_char(buf, '{') < 0)
		return (-1);
	if (append_literal_range(buf, line, start, current) < 0)
		return (-1);
	return (0);
}

int	handle_braced_ctx(t_expand_ctx *ctx, size_t *i)
{
	size_t	start;
	size_t	idlen;
	char	*name;
	int		rc;

	if (!ctx || !i)
		return (-1);
	(*i)++;
	start = *i;
	idlen = parse_identifier_len(ctx->line, *i, ctx->len);
	*i += idlen;
	if (*i >= ctx->len || ctx->line[*i] != '}')
		return (braced_unclosed_append(ctx, ctx->line, start, *i));
	name = ft_strndup(ctx->line + start, idlen);
	if (!name)
		return (-1);
	(*i)++;
	rc = append_env_by_name(ctx, name);
	free(name);
	return (rc < 0 ? -1 : 0);
}

int	handle_simple_ctx(t_expand_ctx *ctx, size_t *i)
{
	size_t	idlen;
	char	*name;
	int		rc;
	t_buf	*buf;

	if (!ctx || !i)
		return (-1);
	idlen = parse_identifier_len(ctx->line, *i, ctx->len);
	if (idlen == 0)
	{
		buf = ctx->wctx ? &ctx->wctx->buf : ctx->buf;
		if (ctx->wctx)
			ctx->wctx->buf_quoted = ctx->quoted;
		return (buf_append_char(buf, '$') < 0 ? -1 : 0);
	}
	name = ft_strndup(ctx->line + *i, idlen);
	if (!name)
		return (-1);
	*i += idlen;
	rc = append_env_by_name(ctx, name);
	free(name);
	return (rc < 0 ? -1 : 0);
}