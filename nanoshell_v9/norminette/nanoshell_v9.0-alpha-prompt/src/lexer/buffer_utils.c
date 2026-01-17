/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	buf_append_str(t_buf *b, const char *s)
{
	size_t	slen;

	if (!b || !s)
		return (0);
	slen = ft_strlen(s);
	if (slen == 0)
		return (0);
	if (buf_ensure_capacity(b, b->len + slen + 1) != 0)
		return (-1);
	ft_memcpy(b->data + b->len, s, slen);
	b->len += slen;
	b->data[b->len] = '\0';
	return (0);
}

void	buf_init(t_buf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

void	buf_free(t_buf *b)
{
	if (!b)
		return ;
	free(b->data);
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

static size_t	next_capacity(size_t cur, size_t min_needed)
{
	size_t	ncap;

	if (min_needed == 0)
		return (0);
	if (cur == 0)
		ncap = 32;
	else
	{
		ncap = cur * 2;
	}
	while (ncap < min_needed)
	{
		if (ncap > SIZE_MAX / 2)
		{
			ncap = min_needed;
			break ;
		}
		ncap *= 2;
	}
	if (ncap < min_needed)
		ncap = min_needed;
	return (ncap);
}

int	buf_ensure_capacity(t_buf *b, size_t min_needed)
{
	size_t	ncap;
	char	*n;

	if (!b)
		return (-1);
	if (b->cap >= min_needed)
		return (0);
	ncap = next_capacity(b->cap, min_needed);
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

int	buf_append_char(t_buf *b, char c)
{
	size_t	required;

	if (!b)
		return (-1);
	required = b->len + 2;
	if (buf_ensure_capacity(b, required) != 0)
		return (-1);
	b->data[b->len++] = c;
	b->data[b->len] = '\0';
	return (0);
}

char	*buf_release(t_buf *buf)
{
	char	*ret;

	if (!buf)
		return (NULL);
	if (buf->data == NULL)
	{
		ret = malloc(1);
		if (!ret)
			return (NULL);
		ret[0] = '\0';
		buf->len = 0;
		buf->cap = 0;
		return (ret);
	}
	ret = buf->data;
	buf->data = NULL;
	buf->len = 0;
	buf->cap = 0;
	return (ret);
}
