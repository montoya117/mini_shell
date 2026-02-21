/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:14:55 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:14:58 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

/* ** 1. Sort helper (Bubble sort on a copy)
*/
static char	**get_sorted_env(char **envp)
{
	char	**tmp;
	int		i;
	int		j;

	tmp = dup_env(envp);
	if (!tmp)
		return (NULL);
	i = 0;
	while (tmp[i])
	{
		j = i + 1;
		while (tmp[j])
		{
			if (ft_strcmp(tmp[i], tmp[j]) > 0)
				ft_swap_str(&tmp[i], &tmp[j]);
			j++;
		}
		i++;
	}
	return (tmp);
}

/* ** 2. Print logic without extra mallocs
** Using printf precision (%.*s) prevents creating a substring just to print it
*/
static void	print_export_line(char *env_str)
{
	char	*eq;
	int		len;

	eq = ft_strchr(env_str, '=');
	if (eq)
	{
		len = (int)(eq - env_str);
		printf("declare -x %.*s=\"%s\"\n", len, env_str, eq + 1);
	}
	else
		printf("declare -x %s\n", env_str);
}

/* ** 3. Process individual argument
*/
static int	process_export_arg(t_data *data, char *arg)
{
	char	*eq;
	char	*name;
	char	*val;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		name = ft_substr(arg, 0, eq - arg);
		val = ft_strdup(eq + 1);
	}
	else
	{
		name = ft_strdup(arg);
		val = NULL;
	}
	if (!is_valid_identifier(name))
	{
		builtin_error("export", arg, "not a valid identifier");
		return (free(name), free(val), 1);
	}
	if (val != NULL)
		set_env_var_data(data, name, val);
	else if (find_name(data->envp, name) < 0)
		set_env_var_data(data, name, "");
	return (free(name), free(val), 0);
}

/* ** 4. Main Export Builtin
*/
int	builtin_export(char **argv, t_data *data)
{
	int		i;
	int		status;
	char	**sorted;

	status = 0;
	if (!argv || !argv[1])
	{
		sorted = get_sorted_env(data->envp);
		if (!sorted)
			return (1);
		i = 0;
		while (sorted[i])
			print_export_line(sorted[i++]);
		free_env(sorted);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		if (process_export_arg(data, argv[i]) != 0)
			status = 1;
		i++;
	}
	return (status);
}

/*
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
*/
