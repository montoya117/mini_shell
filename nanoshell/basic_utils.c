/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:57:34 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/08 14:33:27 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '<' || c == '<' || c == '|')
		return (1);
	retur  (0);
}

void    skip_spaces(const char *line, size_t *i, size_t len)
{
    while (*i < len && is_space(line[*i]))
        (*i)++;
}
