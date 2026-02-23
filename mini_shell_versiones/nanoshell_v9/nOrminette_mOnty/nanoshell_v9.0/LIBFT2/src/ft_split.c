/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 15:44:46 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 10:13:14 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_numofwrds(char const *s, char c)
{
	int	words_num;
	int	in_word;

	words_num = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			words_num++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (words_num);
}

static char	*ft_cpywrd(const char *start, size_t len)
{
	char	*word;

	word = (char *)malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strlcpy(word, start, len + 1);
	return (word);
}

static void	ft_free_mem(char **list, size_t until)
{
	size_t	i;

	i = 0;
	while (i < until)
	{
		free(list[i]);
		i++;
	}
	if (list)
		free (list);
}

static int	ft_extract_word(char const **s, char c, char **list, size_t *i)
{
	const char	*start;

	while (**s == c)
		(*s)++;
	start = *s;
	while (**s != c && **s != '\0')
		(*s)++;
	if (*s > start)
	{
		list[*i] = ft_cpywrd(start, *s - start);
		if (!list[*i])
			return (-1);
		(*i)++;
		return (1);
	}
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char		**list;
	int			result;
	size_t		i;
	size_t		word_count;

	if (!s)
		return (NULL);
	word_count = ft_numofwrds(s, c);
	list = (char **)ft_calloc(word_count + 1, sizeof(char *));
	if (!list)
		return (NULL);
	i = 0;
	while (*s && i < word_count)
	{
		result = ft_extract_word(&s, c, list, &i);
		if (result == -1)
		{
			ft_free_mem(list, i);
			return (NULL);
		}
	}
	return (list);
}
/*
#include <stdio.h>

int	main()
{
    char *str = "42BCN to the Moon!";
    char **result = ft_split(str, ' ');
    size_t i = 0;

    if (!result)
        return (1);
    while (result[i])
    {
        printf("%s\n", result[i]);
        i++;
    }
    return (0);
}*/
