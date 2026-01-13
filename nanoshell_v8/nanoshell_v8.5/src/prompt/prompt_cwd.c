#include "nanoshell.h"

static char *get_cwd_or_null(void)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}

/* 
    si cwd es NULL devuelve una cadena vacía ("")
    si no hay HOME o cwd no comienza por HOME devuelve una copia de cwd
    si cwd == HOME devuelve "~"
    si cwd está dentro de HOME devuelve "~" seguido del resto.
 */
static char *replace_home_prefix(const char *cwd, const char *home)
{
	size_t home_len;

	if (!home || !cwd)
	{
		if (!cwd)
			return (ft_strdup(""));
		return (ft_strdup(cwd));
	}
	home_len = ft_strlen(home);
	if (ft_strncmp(cwd, home, home_len) != 0)
		return (ft_strdup(cwd));
	if (cwd[home_len] == '\0')
		return (ft_strdup("~"));
	return (ft_strjoin("~", cwd + home_len));
}

char *get_short_cwd(t_data *data)
{
	char        *cwd;
	const char  *home;
	char        *res;

	cwd = get_cwd_or_null();
	if (!cwd)
		return (ft_strdup("?"));
	home = get_var_from_envp(data->envp, "HOME");
	if (!home)
		return (cwd);
	res = replace_home_prefix(cwd, home);
	free(cwd);
	return (res);
}