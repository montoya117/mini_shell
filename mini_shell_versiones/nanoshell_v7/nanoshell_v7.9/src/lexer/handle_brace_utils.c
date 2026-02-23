/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_brace_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:51:32 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:51:40 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

size_t parse_identifier_len(const char *line, size_t i, size_t len)
{
    size_t n;

    if (!line || i >= len)
        return (0);
    if (!((line[i] >= 'A' && line[i] <= 'Z') ||
          (line[i] >= 'a' && line[i] <= 'z') ||
          (line[i] == '_')))
        return (0);
    n = 0;
    while (i + n < len)
    {
        char c = line[i + n];
        if (!((c >= 'A' && c <= 'Z') ||
              (c >= 'a' && c <= 'z') ||
              (c >= '0' && c <= '9') ||
              (c == '_')))
            break ;
        n++;
    }
    return (n);
}

char *ft_strndup(const char *s, size_t n)
{
    char *out;

    if (!s)
        return (NULL);
    out = malloc(n + 1);
    if (!out)
        return (NULL);
    ft_memcpy(out, s, n);
    out[n] = '\0';
    return (out);
}
