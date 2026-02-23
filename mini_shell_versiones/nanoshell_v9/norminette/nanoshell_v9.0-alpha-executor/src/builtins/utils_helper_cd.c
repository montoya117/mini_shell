#include "nanoshell.h"

char *copy_home_and_rest(const char *home, const char *path)
{
	size_t	home_len;
	size_t	path_len;
	size_t	rest;
	size_t	need;
	char	*res;

	if (!home || !path)
		return (NULL);
	home_len = ft_strlen(home);
	path_len = ft_strlen(path);
	if (path_len > 1)
		rest = path_len - 1;
	else
		rest = 0;
	need = home_len + rest + 1;
	res = malloc(need);
	if (!res)
		return (NULL);
	ft_memcpy(res, home, home_len);
	if (rest > 0)
		ft_memcpy(res + home_len, path + 1, rest);
	res[home_len + rest] = '\0';
	return (res);
}

char *expand_home(const char *path, t_data *data)
{
	const char *home;

	if (!path)
		return (NULL);
	if (path[0] != '~')
		return (ft_strdup(path));
	home = get_var_from_envp(data->envp, "HOME");
	if (!home)
	{
		ft_putstr_fd("cd: HOME not set\n", 2);
		return (NULL);
	}
	return (copy_home_and_rest(home, path));
}

char *resolve_cd_target(char **argv, t_data *data)
{
	char *raw;
	char *expanded;

	raw = get_raw_cd_target(argv, data);
	if (!raw)
		return (NULL);
	if (raw[0] == '\0')
	{
		free(raw);
		expanded = expand_home("~", data);
		return (expanded);
	}
	expanded = expand_home(raw, data);
	free(raw);
	return (expanded);
}