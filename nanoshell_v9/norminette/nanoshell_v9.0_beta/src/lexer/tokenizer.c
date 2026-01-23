/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:31:36 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:32:43 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int	append_operator_ctx(t_tokenizer_ctx *ctx, size_t *i, int had_space)
{
	t_token	*op;

	op = parse_operator(ctx->line, i, ctx->len);
	if (!op)
		return (0);
	set_prev_join_next(ctx->head, had_space);
	token_append(ctx->head, op);
	return (1);
}

static int	append_word_ctx(t_tokenizer_ctx *ctx, size_t *i, int had_space)
{
	t_word_src	src;
	t_token		*word;

	src.line = ctx->line;
	src.len = ctx->len;
	src.last_status = ctx->last_status;
	src.data = ctx->data;
	word = parse_word_ctx(&src, i);
	if (!word)
		return (0);
	set_prev_join_next(ctx->head, had_space);
	token_append(ctx->head, word);
	return (1);
}

static int	is_operator_char(char c)
{
	if (c == '<' || c == '>' || c == '|' || c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

static int	process_one_token(t_tokenizer_ctx *ctx, size_t *i)
{
	int	had_space;

	had_space = skip_spaces_with_flag(ctx->line, i, ctx->len);
	if (*i >= ctx->len)
		return (1);
	if (is_operator_char(ctx->line[*i]))
		return (append_operator_ctx(ctx, i, had_space));
	return (append_word_ctx(ctx, i, had_space));
}

t_token	*tokenizer(const char *line, int last_status, t_data *data)
{
	t_tokenizer_ctx	ctx;
	t_token			*head;
	size_t			i;

	if (!line)
		return (NULL);
	ctx.line = line;
	ctx.len = ft_strlen(line);
	ctx.last_status = last_status;
	ctx.data = data;
	head = NULL;
	ctx.head = &head;
	i = 0;
	while (i < ctx.len)
	{
		if (!process_one_token(&ctx, &i))
		{
			free_tokens(head);
			return (NULL);
		}
	}
	return (head);
}
