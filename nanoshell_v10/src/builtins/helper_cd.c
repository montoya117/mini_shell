/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:15:14 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:15:17 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static char	*raw_no_arg(void)
{
	char	*raw;

	raw = ft_strdup("");
	if (!raw)
		return (NULL);
	return (raw);
}

static char	*raw_double_dash(char **argv)
{
	char	*raw;

	if (argv[2])
		raw = ft_strdup(argv[2]);
	else
		raw = ft_strdup("");
	if (!raw)
		return (NULL);
	return (raw);
}

static char	*raw_dash_oldpwd(t_data *data)
{
	const char	*oldpwd;
	char		*raw;

	oldpwd = get_var_from_envp(data->envp, "OLDPWD");
	if (!oldpwd)
	{
		ft_putstr_fd("cd: OLDPWD not set\n", 2);
		return (NULL);
	}
	raw = ft_strdup(oldpwd);
	if (!raw)
		return (NULL);
	return (raw);
}

static char	*raw_from_arg(char **argv)
{
	char	*raw;

	raw = ft_strdup(argv[1]);
	if (!raw)
		return (NULL);
	return (raw);
}

char	*get_raw_cd_target(char **argv, t_data *data)
{
	char	*raw;

	if (!argv || !data)
		return (NULL);
	if (!argv[1])
		return (raw_no_arg());
	if (ft_strncmp(argv[1], "--", 3) == 0)
		return (raw_double_dash(argv));
	if (ft_strncmp(argv[1], "-", 2) == 0)
		return (raw_dash_oldpwd(data));
	raw = raw_from_arg(argv);
	return (raw);
}
