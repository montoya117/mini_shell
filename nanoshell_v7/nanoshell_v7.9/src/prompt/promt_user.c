#include "nanoshell.h"

static int ensure_fallback_q(char **p)
{
	if (*p)
		return (0);
	*p = ft_strdup("?");
	if (!*p)
		return (-1);
	return (0);
}

static char *fetch_env_dup(const char *primary, const char *secondary)
{
	const char *val;
	char *dup;

	val = getenv(primary);
	if (!val && secondary)
		val = getenv(secondary);
	if (!val)
		return (NULL);
	dup = ft_strdup(val);
	return (dup);
}

char *get_user_host(void)
{
	char *user;
	char *host;
	char *tmp;
	char *out;

	user = fetch_env_dup("USER", "LOGNAME");
	host = fetch_env_dup("HOSTNAME", "HOST");
	if (ensure_fallback_q(&user) == -1)
		return (NULL);
	if (ensure_fallback_q(&host) == -1)
	{
		free(user);
		return (NULL);
	}
	tmp = ft_strjoin(user, "@");
	free(user);
	if (!tmp)
	{
		free(host);
		return (NULL);
	}
	out = ft_strjoin(tmp, host);
	free(tmp);
	free(host);
	return (out);
}