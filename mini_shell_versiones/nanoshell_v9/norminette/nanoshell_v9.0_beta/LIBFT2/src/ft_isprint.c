/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:18:16 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/15 10:40:42 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
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
	printf("%d \n",ft_isprint(print));
	return (0);
}*/
