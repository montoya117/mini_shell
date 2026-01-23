/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:02:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:15:50 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	print_escaped(const char *s)
{
	if (!s)
	{
		printf("(null)");
		return ;
	}
	print_escaped_loop(s);
}

void	tokens_print_simple(const t_token *head)
{
	t_token		*cur;
	int			idx;
	const char	*raw;
	const char	*display;

	cur = (t_token *)head;
	idx = 1;
	while (cur)
	{
		raw = cur->text;
		display = display_text_for_token(cur);
		printf("token%-3d type=%-12s pos=%4zu quote=%-6s raw=\"",
			idx, type_to_str(cur->type), cur->pos, quote_to_str(cur->quote));
		print_escaped(raw);
		printf("\" display=\"");
		printf("%s", display);
		if (cur->type == TOKEN_ERROR)
			printf("  <-- ERROR");
		printf("\n");
		cur = cur->next;
		idx++;
	}
}

static void	print_token_header(const t_token *t, size_t i)
{
	printf("token");
	printf("%-3zu", i + 1);
	printf(" type=");
	printf("%-12s", type_to_str(t->type));
	printf(" pos=");
	printf("%4d", (int)t->pos);
	printf(" quote=");
	printf("%-6s", quote_to_str(t->quote));
	printf(" raw=\"");
}

static void	print_token_display_and_close(const t_token *t)
{
	const char	*display;

	printf("\" display=\"");
	display = display_text_for_token(t);
	if (display)
		printf("%s", display);
	printf("\"\n");
}

void	tokens_print_simple_array(const t_token **arr, size_t count)
{
	size_t			i;
	const t_token	*t;

	i = 0;
	while (i < count)
	{
		t = arr[i];
		if (!t)
		{
			print_null_token(i);
		}
		else
		{
			print_token_header(t, i);
			print_escaped(t->text);
			print_token_display_and_close(t);
		}
		i++;
	}
}
