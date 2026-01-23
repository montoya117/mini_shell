
#include "nanoshell.h"

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
