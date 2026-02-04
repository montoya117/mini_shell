/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:15:53 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:39:16 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static void	remove_env_var(t_data *data, int pos)
{
	char	**new_env;
	int		size;
	int		i;
	int		j;

	if (!data->envp)
		return ;
	size = 0;
	while (data->envp[size])
		size++;
	new_env = safe_malloc(size * sizeof(char *));
	i = 0;
	j = 0;
	while (i < size)
	{
		if (i == pos)
			free(data->envp[i]);
		else
			new_env[j++] = data->envp[i];
		i++;
	}
	new_env[j] = NULL;
	free(data->envp);
	data->envp = new_env;
}

int	builtin_unset(char **argv, t_data *data)
{
	int	i;
	int	pos;
	int	status;

	if (!argv || !argv[1] || !data)
		return (0);
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			builtin_error("unset", argv[i], "not a valid identifier");
			status = 1;
		}
		else
		{
			pos = find_name(data->envp, argv[i]);
			if (pos >= 0)
				remove_env_var(data, pos);
		}
		i++;
	}
	return (status);
}

/*
// basically copy all the envp with the same size and then i 
// go thru it and delete the one at the found position correct
// so u re-arrange it
int	builtin_unset(char **argv, t_data *data)
{
	int		status;
	int		i;
	int		pos;
	int		old_i;
	int		new_i;
	int		old_size;
	char	*name;
	char	**new_env;

	if (!argv || !argv[1] || !data)
		return (1);
	i = 1; // 0 is unset
	status = 0;
	while (argv[i]) // for multiple ones
	{
		name = argv[i];
		if (!is_valid_identifier(name))
		{
			builtin_error("unset", name, "not a valid identidier");
		//status = 1;
			i++;
			continue ;
		}
		pos = find_name(data->envp, name);
		if (pos >= 0)
		{
			old_size = 0;
			while (data->envp[old_size])
				old_size++;
			new_env = safe_malloc(old_size * sizeof(char *));
			old_i = 0;
			new_i = 0;
			while (old_i < old_size)
			{
				if (old_i == pos)
					free(data->envp[old_i]);
				else
				{
					new_env[new_i] = data->envp[old_i];
					new_i++;
				}
				old_i++;
			}
			new_env[new_i] = NULL;
			free(data->envp); // frees pointer array only
			data->envp = new_env;
		}
		i++;
	}
	return (status);
}

*/
