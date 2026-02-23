/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 16:36:02 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 08:17:08 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*s1s2;	

	if (!s1 || !s2)
		return (NULL);
	s1s2 = ft_calloc(ft_strlen(s1) + ft_strlen(s2) + 1, sizeof(char));
	if (!s1s2)
		return (NULL);
	i = 0;
	j = 0;
	while (s1[i])
	{
		s1s2[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		s1s2[i + j] = s2[j];
		j++;
	}
	return (s1s2);
}
/*
#include <stdio.h>

int	main()
{
	char	*str1 = "42BCN ";
	char	*str2 = "to the Moon!";

	printf("%s", ft_strjoin(str1, str2));
	return (0);
}*/
