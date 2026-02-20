#include "nanoshell.h"

static int	is_ifs(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(const char *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s && s[i])
	{
		while (s[i] && is_ifs(s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && !is_ifs(s[i]))
			i++;
	}
	return (count);
}

void	free_ifs_split(t_ifs_split r)
{
	free_split(r.words);
}

t_ifs_split	split_unquoted_ifs(const char *s)
{
	t_ifs_split	r;
	int			i;
	int			start;
	int			w;
	int			n;

	r.words = NULL;
	r.leading_ifs = 0;
	r.trailing_ifs = 0;
	if (!s)
		return (r);
	n = (int)ft_strlen(s);
	if (n > 0 && is_ifs(s[0]))
		r.leading_ifs = 1;
	if (n > 0 && is_ifs(s[n - 1]))
		r.trailing_ifs = 1;
	r.words = safe_malloc((count_words(s) + 1) * sizeof(char *));
	i = 0;
	w = 0;
	while (s[i])
	{
		while (s[i] && is_ifs(s[i]))
			i++;
		if (!s[i])
			break ;
		start = i;
		while (s[i] && !is_ifs(s[i]))
			i++;
		r.words[w++] = ft_substr(s, start, i - start);
	}
	r.words[w] = NULL;
	return (r);
}