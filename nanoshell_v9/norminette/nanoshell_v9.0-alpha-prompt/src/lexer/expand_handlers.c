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

static int	append_env_by_name(t_buf *buf, const char *name, t_data *data)
{
	const char	*val;
	int			rc;

	if (!buf || !name || !data)
		return (0);
	val = get_var_from_envp(data->envp, name);
	if (!val)
		return (0);
	rc = buf_append_str(buf, val);
	if (rc < 0)
		return (-1);
	return (0);
}

static int	append_literal_range(t_buf *buf, const char *line, size_t start,
								size_t end)
{
	while (start < end)
	{
		if (buf_append_char(buf, line[start]) < 0)
			return (-1);
		start++;
	}
	return (0);
}

static int	braced_unclosed_append(t_buf *buf, const char *line, size_t start,
									size_t current)
{
	if (!buf || !line)
		return (-1);
	if (buf_append_char(buf, '$') < 0)
		return (-1);
	if (buf_append_char(buf, '{') < 0)
		return (-1);
	if (append_literal_range(buf, line, start, current) < 0)
		return (-1);
	return (0);
}

int	handle_braced(t_buf *buf, const char *line, size_t *i,
	size_t len, t_data *data)
{
	size_t	start;
	size_t	idlen;
	char	*name;
	int		rc;

	if (!buf || !line || !i)
		return (-1);
	(*i)++;
	start = *i;
	idlen = parse_identifier_len(line, *i, len);
	*i += idlen;
	if (*i >= len || line[*i] != '}')
		return (braced_unclosed_append(buf, line, start, *i));
	name = ft_strndup(line + start, idlen);
	if (!name)
		return (-1);
	(*i)++;
	rc = append_env_by_name(buf, name, data);
	free(name);
	if (rc < 0)
		return (-1);
	return (0);
}

int	handle_simple(t_buf *buf, const char *line, size_t *i,
	size_t len, t_data *data)
{
	size_t	idlen;
	char	*name;
	int		rc;

	if (!buf || !line || !i)
		return (-1);
	idlen = parse_identifier_len(line, *i, len);
	if (idlen == 0)
	{
		if (buf_append_char(buf, '$') < 0)
			return (-1);
		return (0);
	}
	name = ft_strndup(line + *i, idlen);
	if (!name)
		return (-1);
	*i += idlen;
	rc = append_env_by_name(buf, name, data);
	free(name);
	if (rc < 0)
		return (-1);
	return (0);
}
