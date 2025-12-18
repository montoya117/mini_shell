/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:50:38 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:50:47 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int expand_special_status(t_buf *buf, size_t *i, int last_status)
{
	char *s;
	int  rc;

	if (!buf || !i)
		return (-1);
	s = ft_itoa(last_status);
	if (!s)
		return (-1);
	rc = buf_append_str(buf, s);
	free(s);
	if (rc < 0)
		return (-1);
	(*i)++;
	return (0);
}

int expand_special_pid(t_buf *buf, size_t *i)
{
	char *s;
	int  rc;

	if (!buf || !i)
		return (-1);
	s = ft_itoa((int)getpid());
	if (!s)
		return (-1);
	rc = buf_append_str(buf, s);
	free(s);
	if (rc < 0)
		return (-1);
	(*i)++;
	return (0);
}

int expand_dollar(t_buf *buf, const char *line, size_t *i, size_t len,
                  int last_status, t_data *data)
{
	if (!buf || !line || !i)
		return (-1);
	if (*i >= len || line[*i] != '$')
		return (0);
	(*i)++;
	if (*i >= len)
	{
		if (buf_append_char(buf, '$') < 0)
			return (-1);
		return (0);
	}
	if (line[*i] == '?')
		return (expand_special_status(buf, i, last_status));
	if (line[*i] == '$')
		return (expand_special_pid(buf, i));
	if (line[*i] == '{')
		return (handle_braced(buf, line, i, len, data));
	return (handle_simple(buf, line, i, len, data));
}
