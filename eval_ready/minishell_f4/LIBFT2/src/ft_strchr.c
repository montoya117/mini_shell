/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 12:59:51 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/13 12:52:51 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (1)
	{
		if (s[i] == (char)c)
			return ((char *)(s + i));
		if (s[i] == '\0')
			break ;
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
int	main()
{
	char	frase[30] = "ABCDEFGHIJK";
	int	letra = 'F';
	printf("%s\n",ft_strchr(frase, letra));
	return (0);
}*/
