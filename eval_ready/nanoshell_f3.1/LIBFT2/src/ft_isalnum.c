/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:04:28 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/15 10:38:26 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	else if ((c >= '0') && (c <= '9'))
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
	printf("%d \n",ft_isalnum(alnum));
	return (0);
}*/
