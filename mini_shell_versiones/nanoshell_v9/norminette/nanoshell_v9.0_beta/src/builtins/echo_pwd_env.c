/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_pwd_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	builtin_env(char **argv, t_data *data)
{
	int	i;

	(void)data;
	if (argv && argv[1])
	{
		ft_putstr_fd("env: no arguments supported\n", 2);
		return (1);
	}
	if (!data || !data->envp)
		return (0);
	i = 0;
	while (data->envp[i])
	{
		ft_putstr_fd(data->envp[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

int	builtin_pwd(char **argv, t_data *data)
{
	char	*cwd;

	(void)argv;
	(void)data;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}

static int	is_n_flag(const char *s)
{
	int	i;

	i = 1;
	if (!s || s[0] != '-' || s[1] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_echo(char **argv, t_data *data)
{
	int	i;
	int	no_n;

	(void)data;
	i = 1;
	no_n = 0;
	while (argv && argv[i] && is_n_flag(argv[i]))
	{
		no_n = 1;
		i++;
	}
	while (argv && argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (!no_n)
		ft_putstr_fd("\n", 1);
	return (0);
}
