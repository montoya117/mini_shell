/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 11:19:58 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/18 13:05:04 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static void	ft_cpyforward(
	unsigned char *dst, const unsigned char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dst[i] = src[i];
		i++;
	}
}

static void	ft_cpybackward(unsigned char *dst, const void *src, size_t n)
{
	const unsigned char	*s;

	s = (const unsigned char *)src;
	while (n-- > 0)
		dst[n] = s[n];
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr_src;
	unsigned char	*ptr_dest;

	ptr_src = (unsigned char *)src;
	ptr_dest = (unsigned char *)dest;
	if (n == 0 || dest == src)
		return (dest);
	if (ptr_dest < ptr_src)
		ft_cpyforward(ptr_dest, ptr_src, n);
	else
		ft_cpybackward(ptr_dest, ptr_src, n);
	return (dest);
}
/*
#include <stdio.h>

int	main()
{
	char	dst[30];
	char	src[] = "42BCN to the Moon my Frate!";
	ft_memmove(dst, src, 10);
	printf("%s\n", dst);
	return (0);
}*/
