/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_token_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:02:42 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:15:50 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	print_escaped_loop(const char *s)
{
	unsigned char	c;

	while (*s)
	{
		c = (unsigned char)*s;
		print_escaped_char(c);
		s++;
	}
}

void	print_null_token(size_t i)
{
	printf("token");
	printf("%zu", i + 1);
	printf(": (NULL)\n");
}
