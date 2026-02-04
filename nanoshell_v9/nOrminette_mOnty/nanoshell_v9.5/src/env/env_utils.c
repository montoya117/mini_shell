#include "nanoshell.h"

char	**dup_env(char **envp)
{
	int		count;
	char	**copy;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	copy = safe_malloc((count + 1) * sizeof(char *));
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

static int	append_env(char ***penv, char *new_entry)
{
	int		count;
	int		i;
	char	**new_env;

	count = 0;
	while ((*penv)[count])
		count++;
	new_env = safe_malloc((count + 2) * sizeof(char *));
	if (!new_env)
		return (1);
	i = 0;
	while (i < count)
	{
		new_env[i] = (*penv)[i];
		i++;
	}
	new_env[count] = new_entry;
	new_env[count + 1] = NULL;
	free(*penv);
	*penv = new_env;
	return (0);
}

int	set_env_var_generic(char ***penv, const char *name, const char *value)
{
	int		i;
	char	*new_entry;

	if (!penv || !name || !value)
		return (1);
	new_entry = join_name_value(name, value);
	if (!new_entry)
		return (1);
	i = find_name(*penv, name);
	if (i >= 0)
	{
		free((*penv)[i]);
		(*penv)[i] = new_entry;
		return (0);
	}
	if (append_env(penv, new_entry))
	{
		free(new_entry);
		return (1);
	}
	return (0);
}

// wrapper fo r persistent cases
int	set_env_var_data(t_data *data, const char *name, const char *value)
{
	return (set_env_var_generic(&data->envp, name, value));
}

const char	*get_var_from_envp(char **envp, const char *name)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	free_env(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}
