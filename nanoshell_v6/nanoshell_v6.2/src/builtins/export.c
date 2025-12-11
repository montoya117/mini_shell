
#include "nanoshell.h"

int is_valid_identifier(char *arg)
{
    int i;

    if (!arg || arg[0] == '\0')
        return (0);
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (0);
    i = 1;
    while (arg[i])
    {
        if ( !ft_isalnum(arg[i]) && arg[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

// hace el set de *name y *value pero no los retorna
void split_name_value(const char *arg, char **name, char **value)
{
    char    *equal;

    if (!arg || !name || !value)
        return ;
    equal = ft_strchr(arg, '=');
    if (!equal)
    {
        *name = ft_strdup(arg);
        if (!*name)
        {
            *value = NULL;
            return;
        }
        *value = NULL;
        return ;
    }
    *name = ft_substr(arg, 0, equal - arg);
    if (!*name)
    {
        *value = NULL;
        return;
    }
    *value = ft_strdup(equal + 1);
    if (!*value)
    {
        free(*name);
        *name = NULL;
        return ;
    }
}

int builtin_export(char **argv, t_data *data)
{
    int     i;
    int     status;
    char    *name;
    char    *value;

    name = NULL;
    value = NULL;
    status = 0;
    // argv[0] == export 
    // case of export with no arguments
    if (argv[1] == NULL)
    {
        //print_sorted_env(data->envp) TODO
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
            builtin_error("export", argv[i], "not a vlaid identifier");
            status = 1;
        }
        else
        {
            if (value == NULL)
                set_env_var_data(data, name , ""); // export NAME
            else
                set_env_var_data(data, name, value); // export NAME=VALUE
        }
        free(name);
        free(value);
        i++;
    }
    data->last_status = status;
    return (status);
}
