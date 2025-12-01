/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:01:01 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/19 08:19:24 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>

void	funsion(unsigned int i, char *str)
{
	if (i % 2 == 0)
		*str = ft_toupper(*str);
	else
		*str = ft_tolower(*str);
}

int	main()
{
	char	str[] = "42 To the moon!";

	ft_striteri(str, funsion);
	printf("%s\n", str);
	return (0);
}*/
