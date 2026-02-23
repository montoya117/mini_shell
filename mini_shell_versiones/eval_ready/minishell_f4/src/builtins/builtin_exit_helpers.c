/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 12:53:34 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/23 12:53:38 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	get_sign_and_index(const char *s, int *i)
{
	int	sign;

	sign = 1;
	*i = 0;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

int	will_overflow_pos(long acc, int d)
{
	if (acc > (LONG_MAX - d) / 10)
		return (1);
	return (0);
}

int	will_overflow_neg(long acc, int d)
{
	if (-acc < (LONG_MIN + d) / 10)
		return (1);
	return (0);
}

int	parse_long_checked(const char *s, long *out)
{
	long	acc;
	int		i;
	int		sign;
	int		d;

	if (!s || !out)
		return (0);
	sign = get_sign_and_index(s, &i);
	acc = 0;
	while (s[i])
	{
		d = s[i] - '0';
		if (sign == 1 && will_overflow_pos(acc, d))
			return (0);
		if (sign == -1 && will_overflow_neg(acc, d))
			return (0);
		acc = acc * 10 + d;
		i++;
	}
	*out = acc * sign;
	return (1);
}
