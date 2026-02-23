/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:31:36 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:32:43 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	skip_spaces_with_flag(const char *line, size_t *i, size_t len)
{
	size_t	before;

	before = *i;
	skip_spaces(line, i, len);
	if (*i > before)
		return (1);
	return (0);
}

void	set_prev_join_next(t_token **head, int had_space)
{
	t_token	*prev;

	prev = *head;
	while (prev && prev->next)
		prev = prev->next;
	if (!prev)
		return ;
	if (had_space)
		prev->join_next = 0;
	else
		prev->join_next = 1;
}
