
#include "nanoshell.h"

char    **dup_env(char **envp)
{
    int     count;
    char    **copy;
    int     i;

    count = 0;
    while(envp[count])
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

int set_env_var_generic(char ***penv, const char *name, const char *value)
{
    int     i;
    int     count;
    char    *new_entry;
    char    **new_env;

    if (!penv || !name || !value)
        return (1);
    new_entry = join_name_value(name, value);
    if (!new_entry)
        return (1);
    // 1) Try to replace existing
    i = find_name(*penv, name);
    if (i >= 0) //so if there was name, other wise i=0
    {
        free((*penv)[i]);
        (*penv)[i] = new_entry;
        return (0);
    }
    // 2) Append new variable
    count = 0;
    while ((*penv)[count])
        count++;
    new_env = safe_malloc((count + 2) * sizeof(char *));
    i = 0;
    while (i < count)
    {
        new_env[i] = (*penv)[i]; // just copy pointers
        i++;
    }
    new_env[count] = new_entry;
    new_env[count + 1] = NULL;

    // free old array (not strings)
    free(*penv);
    *penv = new_env;
    return (0);
}

// wrapper fo r persistent cases
int set_env_var_data(t_data *data, const char *name, const char *value)
{
    return (set_env_var_generic(&data->envp, name, value));
}

// used in append_env_by_name(t_buf *buf, const char *name, t_data *data) in expand_handlers.c
const char  *get_var_from_envp(char **envp, const char *name)
{
    int i;
    int len;

    i = 0;
    len = ft_strlen(name);
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
            return (envp[i] + len + 1); // points to VALUE
        i++;
    }
    return (NULL); // not found
}

void free_env(char **envp)
{
    int i = 0;

    if (!envp)
        return;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}