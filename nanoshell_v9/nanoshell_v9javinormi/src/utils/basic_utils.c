/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 13:57:34 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 14:30:04 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	is_space(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

int	is_operator(char c)
{
	if (c == '<' || c == '>' || c == '|' ||
		c == '&' || c == '(' || c == ')')
		return (1);
	return (0);
}

void    skip_spaces(const char *line, size_t *i, size_t len)
{
    while (*i < len && is_space(line[*i]))
        (*i)++;
}

void *safe_malloc(size_t size) 
{
    void *ptr = malloc(size);
    if (!ptr) {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int ft_strcmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void    ft_swap_str(char **a, char **b)
{
    char *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

int is_all_digits(const char *s)
{
    int i;

    if (!s || !*s)
        return (0);
    i = 0;
    while (s[i])
    {
        if (!ft_isdigit(s[i]))
            return (0);
        i++;
    }
    return (1);
}