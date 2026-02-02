#include "nanoshell.h"

static int	change_dir_update(const char *path, t_data *data)
{
	char	*prev;
	char	*now;

	if (!path || !data)
		return (1);
	prev = getcwd(NULL, 0);
	if (chdir(path) == -1)
	{
		perror("cd");
		free(prev);
		return (1);
	}
	now = getcwd(NULL, 0);
	if (!now)
	{
		perror("cd: getcwd");
		free(prev);
		return (1);
	}
	if (prev)
		set_env_var_generic(&data->envp, "OLDPWD", prev);
	set_env_var_generic(&data->envp, "PWD", now);
	free(prev);
	free(now);
	return (0);
}

int	builtin_cd(char **argv, t_data *data)
{
	char		*target;
	int			rc;
	const char	*pwd;

	if (!data)
		return (1);
	target = resolve_cd_target(argv, data);
	if (!target)
		return (1);
	rc = change_dir_update(target, data);
	if (rc == 0 && argv[1] && ft_strncmp(argv[1], "-", 2) == 0)
	{
		pwd = get_var_from_envp(data->envp, "PWD");
		if (pwd)
			ft_putendl_fd((char *)pwd, 1);
	}
	free(target);
	return (rc);
}
