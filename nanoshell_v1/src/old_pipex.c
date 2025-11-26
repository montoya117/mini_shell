

/*
int ft_count_words(char *str)
{
    int i = 0;
    int count = 0;

	if (!str)
		return (0);
    while (str[i])
    {
        while(str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
            i++;
        if (str[i] != '\0')
            count++;
        while((str[i] != ' ' && str[i] != '\t' && str[i] != '\n') && str[i] != '\0')
            i++;
    }
    return (count);
}

int ft_len_size(char *str, int init)
{
    int i = 0;

	if (!str)
		return (0);
    while (str[init + i] && (str[init + i] != ' ' && str[init + i] != '\t' && str[init + i] != '\n'))
        i++;
    return (i);
}

void    ft_cpy_str(char *ori, char *dest, int init)
{
    int i = 0;
	if (!ori || !dest)
		return  ;
    while (ori[i + init] && (ori[i + init] != ' ' && ori[i + init] != '\t' && ori[i + init] != '\n'))
    {
        dest[i] = ori[init + i];
        i++;
    }
    dest[i] = '\0';
}

char    **ft_split(char *str)
{
    int i = 0;
    int j = 0;
    int nwords = ft_count_words(str);
    int len = 0;
    char **out;

	if (!str)
		return (NULL);
    out = malloc(sizeof(char *) * (nwords + 1));
    if (!out)
    {
        perror("error split malloc");
        return (NULL);
    }
    while (str[i])
    {
        while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
            i++;
        len = ft_len_size(str, i);
		if (len == 0)
		{
			fprintf(stderr, "error in len_size");
			out[j] = NULL;
			ft_free_all(out);
			return (NULL);
		}
		if (j >= nwords)
			break ;
        out[j] = malloc(sizeof(char) * (len + 1));
        if (!out[j])
        {
            perror("error allocating split malloc");
            out[j] = NULL; // LO PONEMOS A NULL PARA Q LA FUNC. FREE_ALL ENCUENTRE DONDE TERMINAR DE ITERAR.
			ft_free_all(out);
            return (NULL);
        }
        ft_cpy_str(str, out[j], i);
        j++;
        i += len;
    }
    out[j] = NULL;
    return (out);
}

void	ft_free_all(char **ar)
{
	int	i = 0;
	if (!ar)
		return ;
	while (ar[i])
	{
		free(ar[i]);
		i++;
	}
	free(ar);
}

char	*join_path_cmd(char *path, char *cmd)
{
	char	*out;
	int		len1;
	int		len2;
	int		i = 0;
	int		j = 0;

	if (!path || !cmd)
		return (NULL);
	len1 = strlen(path);
	len2 = strlen(cmd);
	out = malloc(sizeof(char ) * (len1 + len2 + 2));
	if (!out)
	{
		perror ("error joining path & cmd");
		return (NULL);
	}
	if (path)
	{
		while (path[i])
		{
			out[i] = path[i];
			i++;
		}
	}
	out[i++] = '/';
	if (cmd)
	{
		while (cmd[j])
		{
			out[i +  j] = cmd[j];
			j++;
		}
	}
	out[i + j] = '\0';
	return (out);
}

int	ft_count_paths(char *raw)
{
	int	i = 0;
	int	count = 1;

	if (!raw)
		return (0);
	while (raw[i])
	{
		if (raw[i] == ':')
			count++;
		i++;
	}
	return (count);
}

void		ft_cpy_path(char *raw, char *dest, int start)
{
	int	i = 0;

	if (!raw || !dest)
		return ;
	while (raw[i + start] && raw[i + start] != ':')
	{
		dest[i] = raw[i + start];
		i++;
	}
	dest[i] = '\0';
}

char	**split_path(char *raw)
{
	int		i = 0;
	int		k = 0;
	int		start = 0;
	int		posibles = 0;
	int		j = 0;
	int		len = 0;
	char	**out;

	if (!raw)
		return (NULL);
	posibles = ft_count_paths(raw);
	out = malloc(sizeof(char *) * (posibles + 1));
	if (!out)
	{
		perror("error allocating path arrays");
		return (NULL);
	}
	while (1)
	{
		if (raw[i] == ':' || raw[i] == '\0')
		{
			len = i - start;
			if (j >= posibles)
				break ;
			out[j] = malloc(sizeof(char) * (len + 1));
			if (!out[j])
			{
				perror("error allocating inisde the array");
				out[j] = NULL;//lo ponemos a null para que la funcion free_all encuentre el final de la iteracion.
				ft_free_all(out);
				return (NULL);
			}
			ft_cpy_path(raw, out[j], start);
			j++;
			if (raw[i] == '\0')
				break ;
			start = i + 1;
		}
		i++;
	}
	out[j] = NULL;
	return (out);

}

char	*find_path(char *cmd)
{
	char	*env_path = getenv("PATH");
	char	**all_paths;
	int		i = 0;
	char	*full_path;
    
    if (!env_path)
        return (NULL);
	if (!cmd)
		return(NULL);
	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));
	if (!env_path)
		return (NULL);
	all_paths = split_path(env_path);
	if (!all_paths)
		return (NULL);
	while (all_paths[i])
	{
		full_path = join_path_cmd(all_paths[i], cmd);
		if (!full_path)
		{
			//free(full_path);(innecesario?)
            ft_free_all(all_paths);
			return NULL;
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_free_all(all_paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_all(all_paths);
	return (NULL);
}

void	execute_cmd(char *line_cmd, char **ev)
{
	
	char	**cmd_args;
	char	*path;

	if (!line_cmd)
		_exit (0);
	cmd_args = ft_split(line_cmd);
	if (!cmd_args || !cmd_args[0])
	{
		ft_free_all(cmd_args);
		free(line_cmd);
		_exit(0);
	}
	path = find_path(cmd_args[0]);
	if (!path)
	{
		fprintf(stderr, "%s: command not found\n", cmd_args[0]);
		ft_free_all(cmd_args);
		free(line_cmd);
		_exit(127);
	}
    if (execve(path, cmd_args, ev) == -1)
    {
        perror("execve failed");
        free(path);
        ft_free_all(cmd_args);
		free(line_cmd);
        _exit(127);
    }
}
*/