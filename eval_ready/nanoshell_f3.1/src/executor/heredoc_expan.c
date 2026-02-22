
#include "nanoshell.h"

static void	process_heredoc_var(char *line, size_t *i, t_data *data, t_buf *buf)
{
	size_t		start;
	char		*name;
	const char	*val;
	char		*status_str;

	(*i)++; // Saltamos el '$'
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
	name = ft_strndup(&line[start], *i - start); // Usa tu ft_strndup
	val = get_var_from_envp(data->envp, name);   // Usa tu función exacta
	if (val)
		buf_append_str(buf, val);                // Usa tu buffer helper
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
	return (buf_release(&buf)); // Tu función que devuelve la string terminada
}