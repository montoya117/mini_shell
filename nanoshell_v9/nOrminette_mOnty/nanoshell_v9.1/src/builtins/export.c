#include "nanoshell.h"

int	is_valid_identifier(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
		return (0);
	if (!ft_isalpha(arg[0]) && arg[0] != '_')
		return (0);
	i = 1;
	while (arg[i])
	{
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

// hace el set de *name y *value pero no los retorna
void	split_name_value(const char *arg, char **name, char **value)
{
	char	*equal;

	if (!arg || !name || !value)
		return ;
	equal = ft_strchr(arg, '=');
	if (!equal)
	{
		*name = ft_strdup(arg);
		if (!*name)
		{
			*value = NULL;
			return ;
		}
		*value = NULL;
		return ;
	}
	*name = ft_substr(arg, 0, equal - arg);
	if (!*name)
	{
		*value = NULL;
		return ;
	}
	*value = ft_strdup(equal + 1);
	if (!*value)
	{
		free(*name);
		*name = NULL;
		return ;
	}
}

void	print_sorted_env(char **envp)
{
	char	**tmp;
	char	*name;
	char	*value;
	int		count;
	int		i;
	int		j;

	count = 0;
	while (envp[count])
		count++;
	tmp = dup_env(envp);
	i = 0;
	while (i < count)
	{
		j = i + 1;
		while (j < count)
		{
			if (ft_strcmp(tmp[i], tmp[j]) > 0)
			{
				ft_swap_str(&tmp[i], &tmp[j]);
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < count)
	{
		name = NULL;
		value = NULL;
		split_name_value(tmp[i], &name, &value);
		if (!name)
		{
			i++;
			continue ;
		}
		if (!value)
			printf("declare -x %s\n", name);
		else
			printf("declare -x %s=\"%s\"\n", name, value);
		free(name);
		free(value);
		i++;
	}
	free_env(tmp);
}

int	builtin_export(char **argv, t_data *data)
{
	int		i;
	int		status;
	char	*name;
	char	*value;

	name = NULL;
	value = NULL;
	status = 0;
	if (argv[1] == NULL)
	{
		print_sorted_env(data->envp);
		data->last_status = 0;
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		split_name_value(argv[i], &name, &value);
		if (!name)
		{
			status = 1;
			data->last_status = 1;
			i++;
			continue ;
		}
		if (!is_valid_identifier(name))
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		else
		{
			if (value == NULL)
				set_env_var_data(data, name, "");
			else
				set_env_var_data(data, name, value);
		}
		free(name);
		free(value);
		i++;
	}
	data->last_status = status;
	return (status);
}

/*
function print_sorted_declare_x(envp):
	# 1) count n
	# 2) duplicate envp -> tmp
	# 3) bubble sort tmp with strcmp
	# 4) for each tmp[i]:
	#       split at '='
	#       print "declare -x NAME=\"VALUE\""
	# 5) free tmp strings + array
*/