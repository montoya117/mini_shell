/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:56:13 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:56:18 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static char *join_dir_cmd(const char *dir, const char *cmd)
{
	size_t	dir_len;
	size_t	cmd_len;
	size_t	add_slash;
	size_t	need;
	char	*out;

	if (!calc_need_and_slash(dir, cmd, &need, &add_slash))
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	out = malloc(need);
	if (!out)
		return (NULL);
	if (dir_len > 0)
		ft_memcpy(out, dir, dir_len);
	if (add_slash)
		out[dir_len] = '/';
	ft_memcpy(out + dir_len + add_slash, cmd, cmd_len);
	out[dir_len + add_slash + cmd_len] = '\0';
	return (out);
}

int  try_path_segment(const char *seg, const char *cmd, char ** out)
{
    char        *candidate;
    const char  *dir;

    if (!seg || !cmd || !out)
        return (0);
    if (*seg == '\0')
        dir = ".";
    else
        dir = seg;
    candidate = join_dir_cmd(dir, cmd);
    if (!candidate)
        return (0);
    if (access(candidate, X_OK) == 0)
    {
        *out = candidate;
        return (1);
    }
    free(candidate);
    return (0);
}

static char *search_path_segments(const char *path, const char *cmd)
{
    size_t  len;
    size_t  start;
    size_t  i;
    char    *res;
    int     rc;

    if (!path || !cmd)
        return (NULL);
    len = ft_strlen(path);
    start = 0;
    while (start <= len)
    {
        i = start;
        while (i < len && path[i] != ':')
            i++;
        rc = try_path_range(path, start, i, cmd, &res);
        if (rc == 1)
            return (res);
        if (rc == -1)
            return (NULL); /* propagar error de malloc */
        start = i + 1;
    }
    return (NULL);
}

static char *get_envp_path(char **envp)
{
    int i;

    if (!envp)
        return (NULL);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
        i++;
    }
    return (NULL);
}

char    *find_path(char *cmd, char **envp)
{
    char    *path;

    if (!cmd || !*cmd)
        return (NULL);
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    path = get_envp_path(envp);
    if (!path)
        return (NULL);
   return (search_path_segments(path, cmd));
}
