/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:22:05 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:22:08 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	check_io_number(int type, t_cmd_info *i)
{
	if ((type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND)
		&& i->last_word
		&& i->last_word->type == TOKEN_WORD
		&& is_all_digits(i->last_word->text)
		&& i->last_word->join_next
		&& !i->last_word->was_quoted)
	{
		i->last_word->is_io_number = 1;
		return (ft_atoi(i->last_word->text));
	}
	return (-1);
}

int	is_redir_token(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	get_default_fd(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
		return (0);
	return (1);
}

/* Ayudante 2: Procesa redirecciones y crea nodos AST */
char	*get_redir_file(int type, char *text)
{
	if (type == TOKEN_HEREDOC)
		return (create_heredoc_tmp(text));
	return (ft_strdup(text));
}

int	count_pieces(const char *s, int was_quoted)
{
	int count;
	int in_word;

	if (was_quoted)
		return (1);
	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s == ' ' || *s == '\t' || *s == '\n')
		{
			if (in_word)
				in_word = 0;
		}
		else
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
			}
		}
		s++;
	}
	return (count);
}