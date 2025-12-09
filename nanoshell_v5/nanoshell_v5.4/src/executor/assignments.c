
#include "nanoshell.h"

void    apply_assignment(const char *assignment, t_data *data)
{
    char    *equal;
    char    *name;
    char    *value;

    equal = ft_strchr(assignment, '=');
    if (!equal)
        return ;
    name = ft_substr(assignment, 0, (equal - assignment));
    value = ft_strdup(equal + 1);
    if (!name || !value)
    {
        free(name);
        free(value);
        return ;
    }
    set_env_var(data, name, value); // TODO
    free(name);
    free(value);
}

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

// helper to look for NAME=VALUE
int is_assignment_word(const char *s)
{
    int i;
	// 1- First char must be letter or '_'
    if (!s || !(ft_isalpha((unsigned char)s[0]) || s[0] == '_'))
        return (0);
    i = 1; // <-- start after the first char
	 // 2- While is a valid NAME move:
	// unsigned char not strictly required, just defensive
    while (s[i] && s[i] != '='
        && (ft_isalnum((unsigned char)s[i]) || s[i] == '_'))
        i++;
	// 3- stop on '='
    if (s[i] != '=')
        return (0);
	// If we reached '=', it's NAME=...
    return (1);
}

int find_name(char **envp, const char *name)
{
    int i;
    int len;

    len = ft_strlen(name);
    i = 0;
    while (envp[i])
    {
        if (!ft_strncmp(envp[i], name, len) && envp[i][len] == '=')
            return i; // return the index
        i++;
    }
    return (-1); // not found
}

char    *join_name_value(const char *name, const char *value)
{
    char    *tmp;
    char    *res;

    if (!name || !value)
        return (NULL);
    tmp = ft_strjoin(name, "=");
    if (!tmp)
        return (NULL);
    res = ft_strjoin(tmp, value);
    free(tmp);
    if (!res)
        return (NULL);
    return (res);
}

int set_env_var(t_data *data, const char *name, const char *value)
{
    int     i;
    int     count;
    char    *new_entry;
    char    **new_env;

    if (!data || !name || !value)
        return (1);
    new_entry = join_name_value(name, value);
    if (!new_entry)
        return (1);
    // 1) Try to replace existing
    i = find_name(data->envp, name);
    if (i >= 0) //so if there was name, other wise i=0
    {
        free(data->envp[i]);
        data->envp[i] = new_entry;
        return (0);
    }
    // 2) Append new variable
    count = 0;
    while (data->envp[count])
        count++;
    new_env = safe_malloc((count + 2) * sizeof(char *));
    i = 0;
    while (i < count)
    {
        new_env[i] = data->envp[i]; // just copy pointers
        i++;
    }
    new_env[count] = new_entry; // i set the assigment value to global envp
    new_env[count + 1] = NULL;

    // free old array (not strings)
    free(data->envp);
    data->envp = new_env;
    return (0);
}

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