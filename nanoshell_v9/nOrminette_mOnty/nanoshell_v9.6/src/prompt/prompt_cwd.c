/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_cwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static char	*get_cwd_or_null(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (NULL);
	return (cwd);
}

/* 
	si cwd es nuull devuelve una cadena vacia ("")
	si no hay homee o cwd no comienza por HOME devuelve una copia delcwd
	si cwd es = al home devuelve ~
	si cwd estaa dentro del HOME devuelve ~ seguido del rest
 */
static char	*replace_home_prefix(const char *cwd, const char *home)
{
	size_t	home_len;

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

char	*get_short_cwd(t_data *data)
{
	char		*cwd;
	const char	*home;
	char		*res;

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
