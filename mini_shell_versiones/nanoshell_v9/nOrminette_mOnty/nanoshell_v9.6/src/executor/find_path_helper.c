/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:57:22 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:57:25 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	calc_need_and_slash(const char *dir, const char *cmd,
	size_t *need, size_t *add_slash)
{
	size_t	dir_len;
	size_t	cmd_len;

	if (!dir || !cmd || !need || !add_slash)
		return (0);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	if (dir_len == 0)
		*add_slash = 1;
	else if (dir[dir_len - 1] == '/')
		*add_slash = 0;
	else
		*add_slash = 1;
	if (cmd_len > SIZE_MAX - dir_len - *add_slash - 1)
		return (0);
	*need = dir_len + *add_slash + cmd_len + 1;
	return (1);
}

int	try_path_range(t_path_range range, const char *cmd, char **res)
{
	char	*seg;
	int		ok;

	if (!range.path || !cmd || !res)
		return (0);
	if (range.end == range.start)
	{
		if (try_path_segment("", cmd, res))
			return (1);
		return (0);
	}
	seg = ft_strndup(range.path + range.start, range.end - range.start);
	if (!seg)
		return (-1);
	ok = try_path_segment(seg, cmd, res);
	free(seg);
	if (ok)
		return (1);
	return (0);
}
