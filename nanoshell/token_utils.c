/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 14:35:49 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/08 15:22:19 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

t_token	*token_new(t_token_type type, char *text, t_quote_type qt, int pos)
{
	t_token	*t = malloc(sizeof(t_token));
	if (!t)
	{
		free(text);
		return (NULL);
	}
	t->type = type;
	t->text = text;
	t->quote = qt;
	t->pos = pos;
	t->next = NULL;
	return	(t);
}

void	token_append(t_token **head, t_token *node)
{
	t_token	*cur;

	if (!node)
		return ;
	if (!*head)
	{
		*head = node;
		return ;
	}
	cur = head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	free_tokens(t_token *head)
{
	t_token *cur;
	t_token	*n;

	cur = head;
	while (cur)
	{
		n = cur->next;
		if (cur->text)
			free(cur->text);
		free(cur);
		cur = n;
	}
}
