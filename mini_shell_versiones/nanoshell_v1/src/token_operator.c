/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:00:48 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 14:16:58 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static t_token *handle_greater(const char *line, size_t *i, size_t len, size_t pos)
{
    if (*i + 1 < len && line[*i + 1] == '>')
    {
        *i += 2;
        return (token_new(TOKEN_REDIR_APPEND, ft_strdup(">>"), QT_NONE, pos));
    }
    *i += 1;
    return (token_new(TOKEN_REDIR_OUT, ft_strdup(">"), QT_NONE, pos));
}
static t_token	*handle_less(const char *line, size_t *i, size_t len, size_t pos)
{
    if (*i + 1 < len && line[*i + 1] == '<' )
    {
        *i += 2;
        return (token_new(TOKEN_HEREDOC, ft_strdup("<<"), QT_NONE, pos));
    }
    *i += 1;
    return (token_new(TOKEN_REDIR_IN, ft_strdup("<"), QT_NONE, pos));
}

static t_token	*handle_pipe_or_or(const char *line, size_t *i, size_t len, size_t pos)
{
    if (*i + 1 < len && line[*i + 1] == '|')
	{
		*i += 2;
		return (token_new(TOKEN_OR, ft_strdup("||"), QT_NONE, pos));
	}
	*i += 1;
    return (token_new(TOKEN_PIPE, ft_strdup("|"), QT_NONE, pos));
}

static t_token	*handle_and_or_amp(const char *line, size_t *i, size_t len, size_t pos)
{
	if (*i + 1 < len && line[*i + 1] == '&')
	{
		*i += 2;
		return (token_new(TOKEN_AND, ft_strdup("&&"), QT_NONE, pos));
	}
	*i += 1;
	return (token_new(TOKEN_WORD, ft_strdup("&"), QT_NONE, pos));
}

static t_token	*handle_paren(const char *line, size_t *i, size_t len, size_t pos)
{
	(void)len;
	if (line[*i] == '(')
	{
		*i += 1;
		return (token_new(TOKEN_LPAREN, ft_strdup("("), QT_NONE, pos));
	}
	*i += 1;
	return (token_new(TOKEN_RPAREN, ft_strdup(")"), QT_NONE, pos));
}

t_token *parse_operator(const char *line, size_t *i, size_t len)
{
    size_t pos;

    if (!line || !i)
        return (NULL);
    pos = *i;
    if (line[*i] == '>') {
		return (handle_greater(line, i, len, pos));}
    if (line[*i] == '<') {
        return (handle_less(line, i, len, pos));}
    if (line[*i] == '|') {
        return (handle_pipe_or_or(line, i, len, pos));}
	if (line[*i] == '&') {
		return (handle_and_or_amp(line, i, len, pos));}
	if (line[*i] == '(' || line[*i] == ')'){
		return (handle_paren(line, i, len, pos));}
    return (NULL);
}
