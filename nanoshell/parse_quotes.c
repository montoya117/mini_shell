/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 13:50:51 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 14:55:05 by jadelgad         ###   ########.fr       */
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

int	parse_double_quote(t_buf *buf, const char *line, size_t *i, size_t len)
{
	while (*i < len && line[*i] != '"')
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
