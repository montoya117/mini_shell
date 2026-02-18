/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_user.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"
#define HOST_READ_MAX 256

/* Read up to HOST_READ_MAX-1 bytes from path and return trimmed first-line. */
static char	*read_file_first_line(const char *path)
{
	int		fd;
	ssize_t	n;
	char	buf[HOST_READ_MAX];

	if (!path)
		return (NULL);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (n <= 0)
		return (NULL);
	buf[n] = '\0';
	return (trim_and_dup(buf, (size_t)n));
}

/* Duplicate env value or return NULL. */
static char	*dup_env_var(const char *primary, const char *secondary)
{
	const char	*value;

	if (!primary)
		return (NULL);
	value = getenv(primary);
	if (!value && secondary)
		value = getenv(secondary);
	if (!value)
		return (NULL);
	return (ft_strdup(value));
}

/* Return malloc'd username or "?" (caller frees). */
static char	*get_user_from_env(void)
{
	char	*user;

	user = dup_env_var("USER", "LOGNAME");
	if (!user)
		user = ft_strdup("?");
	return (user);
}

/* Return malloc'd hostname or "?" (caller frees). Tries env then files. */
static char	*get_host_from_env_or_files(void)
{
	char	*host;

	host = dup_env_var("HOSTNAME", "HOST");
	if (host)
		return (host);
	host = read_file_first_line("/etc/hostname");
	if (host)
		return (host);
	host = read_file_first_line("/proc/sys/kernel/hostname");
	if (host)
		return (host);
	return (ft_strdup("?"));
}

/*returns "user@host" malloc'd (caller must free) or NULL on alloc err */
char	*get_user_host(void)
{
	char	*user;
	char	*host;
	char	*tmp;
	char	*out;

	user = get_user_from_env();
	host = get_host_from_env_or_files();
	if (!user || !host)
	{
		free(user);
		free(host);
		return (NULL);
	}
	tmp = ft_strjoin(user, "@");
	if (!tmp)
	{
		free(user);
		free(host);
		return (NULL);
	}
	out = ft_strjoin(tmp, host);
	free(tmp);
	free(user);
	free(host);
	return (out);
}
