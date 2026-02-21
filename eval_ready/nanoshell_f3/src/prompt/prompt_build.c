/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_build.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/19 14:41:41 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

/* calcula la suma de longitude
deevuelve 0 si count <= 0
 */
static size_t	parts_total_len(const char **parts, int count)
{
	size_t	total;
	size_t	i;

	if (count <= 0)
		return (0);
	total = 0;
	i = 0;
	while (i < (size_t)count)
	{
		total += ft_strlen(parts[i]);
		i++;
	}
	return (total);
}

/* copiamoos todas las partes en el buffer 'out' necsiita espacio suficiente
al final pone '\0'
 */
static void	parts_copy_to_buf(const char **parts, int count, char *out)
{
	size_t	pos;
	size_t	i;
	size_t	len;

	pos = 0;
	i = 0;
	while (i < (size_t)count)
	{
		len = ft_strlen(parts[i]);
		if (len > 0)
		{
			ft_memcpy(out + pos, parts[i], len);
			pos += len;
		}
		i++;
	}
	out[pos] = '\0';
}

/* une count partes en una unica cadeena alocada una vez
devuelve heap (caller free) o null en fallo
 */
char	*join_parts(const char **parts, int count)
{
	size_t	total;
	char	*out;

	if (count <= 0)
		return (ft_strdup(""));
	total = parts_total_len(parts, count);
	out = malloc(total + 1);
	if (!out)
		return (NULL);
	parts_copy_to_buf(parts, count, out);
	return (out);
}
//buclee para ir uniendo todas las partes

static int	prompt_build_parts(t_data *data, char **userhost_out,
	char **shortcwd_out)
{
	char	*userhost;
	char	*shortcwd;

	userhost = get_user_host();
	shortcwd = get_short_cwd(data);
	if (!userhost)
		userhost = ft_strdup("?");
	if (!shortcwd)
		shortcwd = ft_strdup("?");
	if (!userhost || !shortcwd)
	{
		free(userhost);
		free(shortcwd);
		return (-1);
	}
	*userhost_out = userhost;
	*shortcwd_out = shortcwd;
	return (0);
}

char	*build_prompt(t_data *data)
{
	char		*userhost;
	char		*shortcwd;
	const char	*parts[5];
	char		*res;

	if (prompt_build_parts(data, &userhost, &shortcwd) == -1)
		return (NULL);
	parts[0] = userhost;
	parts[1] = ":";
	parts[2] = shortcwd;
	parts[3] = "$> ";
	parts[4] = NULL;
	res = join_parts(parts, 4);
	free(userhost);
	free(shortcwd);
	return (res);
}
