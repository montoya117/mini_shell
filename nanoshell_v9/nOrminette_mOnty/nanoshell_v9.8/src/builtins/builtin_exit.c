/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:52:54 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/11 13:36:30 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int	parse_and_set_code(const char *arg, t_data *data)
{
	long	val;
	int		exit_code;

	val = ft_atoi(arg);
	exit_code = (int)((unsigned char)val);
	if (data)
	{
		data->last_status = exit_code;
		data->running = 1;
	}
	return (exit_code);
}

static int	is_numeric_arg(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	if (s[0] == '+' || s[0] == '-')
		i = 1;
	if (s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (!ft_isdigit((unsigned char)s[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_no_numeric_arg(t_data *data, const char *arg)
{
	ft_putstr_fd("exit: ", 2);
	if (arg)
		ft_putstr_fd((char *)arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	if (data)
	{
		data->last_status = 255;
		data->running = 1;
		return (255);
	}
	return (255);
}

static int	handle_no_args(t_data *data)
{
	if (data)
	{
		data->running = 1;
		return (data->last_status);
	}
	return (0);
}

int	builtin_exit(char **argv, t_data *data)
{
	if (data->pid == 0)
		ft_putendl_fd("exit", 2);
	if (!argv || !argv[1])
		return (handle_no_args(data));
	if (!is_numeric_arg(argv[1]))
		return (handle_no_numeric_arg(data, argv[1]));
	if (argv[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		if (data)
			data->last_status = 1;
		return (data->last_status);
	}
	return (parse_and_set_code(argv[1], data));
}
