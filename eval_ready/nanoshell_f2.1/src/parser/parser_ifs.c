/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ifs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:21:44 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:21:48 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// IFS = espacio, tab, newline
static int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static void	*safe_substr(const char *s, int start, int end)
{
	void	*ptr;

	ptr = ft_substr(s, start, end);
	if (!ptr)
	{
		perror("ft_substr failed");
		exit(EXIT_FAILURE);
	}
	return (ptr);
}

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

char	**split_by_ifs(const char *s)
{
	char	**res;
	int		count;
	int		i;
	int		start;
	int		len;

	if (!s)
		return (NULL);
	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_ifs(s[i]))
			i++;
		if (!s[i])
			break ;
		count++;
		while (s[i] && !is_ifs(s[i]))
			i++;
	}
	res = safe_malloc((count + 1) * sizeof(char *));
	i = 0;
	len = 0;
	start = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] && is_ifs(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		while (s[i] && !is_ifs(s[i]))
			i++;
		len = i - start;
		res[count] = safe_substr(s, start, len);
		count++;
	}
	res[count] = NULL;
	return (res);
}
