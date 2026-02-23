/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_arrays.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alemonto <alemonto@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:56:20 by alemonto          #+#    #+#             */
/*   Updated: 2026/02/19 16:08:41 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static t_token	*fill_assignments(t_cmd_info *info, t_token *tok)
{
	int	i;

	info->assigns = safe_malloc((info->assign_count + 1) * sizeof(char *));
	i = 0;
	while (i < info->assign_count)
	{
		info->assigns[i++] = ft_strdup(tok->text);
		tok = tok->next;
	}
	info->assigns[i] = NULL;
	return (tok);
}

static void	process_word_token(t_cmd_info *info, t_token *tok, int *i)
{
	char	**pieces;
	int		j;

	if (tok->was_quoted)
	{
		info->argv[*i] = ft_strdup(tok->text);
		info->argv_quoted[(*i)++] = 1;
	}
	else
	{
		pieces = split_by_ifs(tok->text);
		j = 0;
		while (pieces && pieces[j])
		{
			info->argv[*i] = ft_strdup(pieces[j]);
			info->argv_quoted[(*i)++] = 0;
			j++;
		}
		free_split(pieces);
	}
}

int	fill_arrays(t_cmd_info *info, t_token *tok)
{
	int	i;

	tok = fill_assignments(info, tok);
	info->argv = safe_malloc((info->argc + 1) * sizeof(char *));
	info->argv_quoted = safe_malloc((info->argc + 1) * sizeof(int));
	i = 0;
	tok = info->words_start;
	while (tok && (tok->type == TOKEN_WORD
			|| tok->type == TOKEN_EXPANSION || is_redir_token(tok->type)))
	{
		if (is_redir_token(tok->type))
		{
			tok = tok->next;
			if (tok)
				tok = tok->next;
			continue ;
		}
		if (!tok->is_io_number)
			process_word_token(info, tok, &i);
		tok = tok->next;
	}
	info->argv[i] = NULL;
	info->argv_quoted[i] = 0;
	return (1);
}
