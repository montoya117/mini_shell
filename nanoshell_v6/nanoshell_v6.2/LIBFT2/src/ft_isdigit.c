/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:12:00 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/15 10:37:02 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
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
	printf("%d \n",ft_isdigit(digit));
	return (0);
}
*/
