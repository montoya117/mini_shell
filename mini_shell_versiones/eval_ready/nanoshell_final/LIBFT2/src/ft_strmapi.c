/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:59:38 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/18 15:58:43 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f) (unsigned int, char))
{
	char	*str;
	size_t	len;
	size_t	i;

	if (!s || !f)
		return (NULL);
	len = ft_strlen(s);
	str = malloc (len + 1);
	if (!str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
/*
#include <stdio.h>
 char    funsion(unsigned int valor, char letra)
 {
	if (valor % 2 == 0)
	letra = ft_toupper(letra);
	else
	letra = ft_tolower(letra);
	 return (letra);
}

int	main()
{
	char	str[] = "42 to the moon!";
	char	*result = ft_strmapi(str, funsion);

	printf("%s\n", result);
	return (0);
}*/
