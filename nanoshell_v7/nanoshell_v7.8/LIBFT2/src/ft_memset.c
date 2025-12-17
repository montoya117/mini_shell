/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 15:58:50 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/15 11:11:35 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	if (n == 0)
		return (s);
	while (i < n)
	{
		ptr[i] = (unsigned char) c;
		i++;
	}
	return (s);
}
/*
#include <stdio.h>

int	main()
{
	char phrase[] = "42BCN";
	char c;

	c = 'X';
	ft_memset(phrase, c, 2);
	printf("%s \n", phrase);
	return (0);
}*/
