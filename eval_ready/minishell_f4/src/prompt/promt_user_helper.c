/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_user_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

/* Trim buffer [0..len) whitespace and return a malloc'd string or NULL. */
char	*trim_and_dup(const char *buf, size_t len)
{
	size_t	start;
	size_t	end;
	char	*out;

	start = 0;
	end = len;
	if (!buf || len == 0)
		return (NULL);
	while (start < end && (buf[start] == ' ' || buf[start] == '\t'
			|| buf[start] == '\r' || buf[start] == '\n'))
		start++;
	while (end > start && (buf[end - 1] == '\n' || buf[end - 1] == '\r'
			|| buf[end - 1] == ' ' || buf[end - 1] == '\t'))
		end--;
	if (start >= end)
		return (NULL);
	out = malloc(end - start + 1);
	if (!out)
		return (NULL);
	ft_memcpy(out, buf + start, end - start);
	out[end - start] = '\0';
	return (out);
}
