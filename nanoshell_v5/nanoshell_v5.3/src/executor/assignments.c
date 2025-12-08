
#include "nanoshell.h"

void    apply_assignment(char ***penv, const char *assignment)
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
    set_env_var_generic(penv, name, value);
    free(name);
    free(value);
}

void    apply_assignments_array(char ***penv, char **assignments)
{
    int i;

    if (!assignments || !penv)
        return ;
    i = 0;
    while (assignments[i])
    {
        apply_assignment(penv, assignments[i]); 
        i++;
    }
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
