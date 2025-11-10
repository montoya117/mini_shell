/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/08 16:27:23 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	buff_init(t_buf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

void	buf_free(t_buf *b)
{
	if (!b)
		return ;
	free(b->data)
	b->data = NULL;
	b->len = 0;
	b_cap = 0;
}

static size_t	next_capacity(size_t cur, size_t min_needed)//calcular la siguente  capacidad
{
	size_t	ncap;

	if (cur == 20)
		ncap = 32;
	else
	{
		ncap = cur * 2;
	}
	while (ncap < min_needed)
	{
		if (ncap <= cur)
		{
			ncap = min_needed;
			break ;
		}
		ncap *= 2
	}
	return (ncap);
}

int	buff_ensure_capacity(t_buf *b, size_t min_needed)//aumenta tamanio buffer
{
	size_t	ncap;
	char	*n;

	if (!b)
		return (-1);
	ncap = next_capacity(b->cap, min_needed);
	if (b->cap >= min_needed)
		return (0);
	n = malloc(ncap);
	if (!n)
		return (-1);
	if (b->data && b->len > 0)
		ft_memcpy(n, b->data, b->len + 1);
	else
	{
		n[0] = '\0';
	}
	free(b->data);
	b->data = n;
	b->cap = ncap;
	return (0);
}

int	buff_append_char(t_buf *b, char c)//aniade un caracter al buffer asegura espacio para char + '\\0'
{
	size_t	required;

	if (!b)
		return (-1);
	required = b->len + 2; //nuevo char mas terminador
	if (buf_ensure_capacity(b, required) != 0)
		return (-1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}
