#include "nanoshell.h"
#define HOST_READ_MAX 256

/* Trim buffer [0..len) whitespace and return a malloc'd string or NULL. */
static char *trim_and_dup(const char *buf, size_t len)
{
	size_t start = 0;
	size_t end = len;
	char *out;

	if (!buf || len == 0)
		return (NULL);
	while (start < end && (buf[start] == ' ' || buf[start] == '\t'
		|| buf[start] == '\r' || buf[start] == '\n'))
		start++;
	while (end > start && (buf[end - 1] == '\n' || buf[end - 1] == '\r'
		|| buf[end - 1] == ' ' || buf[end - 1] == '\t'))
		end--;
	if (start >= end)
		return (NULL);
	out = malloc(end - start + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, buf + start, end - start);
	out[end - start] = '\0';
	return (out);
}

/* Read up to HOST_READ_MAX-1 bytes from path and return trimmed first-line. */
static char *read_file_first_line(const char *path)
{
	int fd;
	ssize_t n;
	char buf[HOST_READ_MAX];

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
static char *dup_env_var(const char *primary, const char *secondary)
{
	const char *value;

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
static char *get_user_from_env(void)
{
	char *user;

	user = dup_env_var("USER", "LOGNAME");
	if (!user)
		user = ft_strdup("?");
	return (user);
}

/* Return malloc'd hostname or "?" (caller frees). Tries env then files. */
static char *get_host_from_env_or_files(void)
{
	char *host;

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

/* Public: returns "user@host" malloc'd (caller must free) or NULL on alloc err */
char *get_user_host(void)
{
	char *user = get_user_from_env();
	char *host = get_host_from_env_or_files();
	char *tmp;
	char *out;

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
