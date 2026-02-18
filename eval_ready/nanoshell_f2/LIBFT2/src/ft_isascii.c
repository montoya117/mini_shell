/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:15:43 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/15 10:39:29 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	else
		return (0);
}
/*
#include <stdio.h>

int	main()
{
	char	alpha;
	char	digit;
	char	alnum;
	char	print;

	alpha = 'a';
	digit = '1';
	alnum = 'a';
	print = '\n';
	printf("%d \n",ft_isscii(alpha));
	return (0);
}*/
