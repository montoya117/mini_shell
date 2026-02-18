/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_error_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:20:02 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:20:04 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

/* free ast_nodes & children */
void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	free_assignments(char **assignments)
{
	int	i;

	if (!assignments)
		return ;
	i = 0;
	while (assignments[i])
	{
		free(assignments[i]);
		i++;
	}
	free(assignments);
}

int	is_token_text(const t_token *token)
{
	if (!token)
		return (0);
	if (token->text && token->text[0] != '\0')
		return (1);
	return (0);
}

const char	*token_display_fallback(const t_token *token)
{
	if (!token)
		return ("newline");
	if (token->type == TOKEN_PIPE)
		return ("|");
	if (token->type == TOKEN_REDIR_OUT)
		return (">");
	if (token->type == TOKEN_REDIR_APPEND)
		return (">>");
	if (token->type == TOKEN_REDIR_IN)
		return ("<");
	if (token->type == TOKEN_HEREDOC)
		return ("<<");
	if (token->type == TOKEN_AND)
		return ("&&");
	if (token->type == TOKEN_OR)
		return ("||");
	if (token->type == TOKEN_LPAREN)
		return ("(");
	if (token->type == TOKEN_RPAREN)
		return (")");
	return ("newline");
}

const char	*token_text_or_newline(const t_token *token)
{
	if (!token || token->type == TOKEN_EOF)
		return ("newline");
	if (is_token_text(token))
		return (token->text);
	return (token_display_fallback(token));
}
