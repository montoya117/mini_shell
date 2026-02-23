/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expan.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 11:19:47 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/23 11:19:57 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	process_heredoc_var(char *line, size_t *i, t_data *data, t_buf *buf)
{
	size_t		start;
	char		*name;
	const char	*val;
	char		*status_str;

	(*i)++;
	start = *i;
	if (line[*i] == '?')
	{
		status_str = ft_itoa(data->last_status);
		buf_append_str(buf, status_str);
		free(status_str);
		(*i)++;
		return ;
	}
	while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
		(*i)++;
	name = ft_strndup(&line[start], *i - start);
	val = get_var_from_envp(data->envp, name);
	if (val)
		buf_append_str(buf, val);
	free(name);
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	t_buf	buf;
	size_t	i;

	buf_init(&buf);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] && (ft_isalnum(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
			process_heredoc_var(line, &i, data, &buf);
		else
			buf_append_char(&buf, line[i++]);
	}
	return (buf_release(&buf));
}
