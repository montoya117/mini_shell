/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_all_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:21:21 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:21:23 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

t_ast	*create_logic_node(t_token_type type, t_ast *left, t_ast *right)
{
	if (type == TOKEN_AND)
		return (ast_new_and(left, right));
	return (ast_new_or(left, right));
}

/*parse_pipe_sequence impide iniciar con redirecciones tras ‘|’.
En bash/minishell sí es válido.
Cambio sugerido: ampliar la condición.*/
t_ast	*parse_pipe_sequence(t_ast *left, t_token **tok,
									t_parser_context *ctx)
{
	t_ast_list	*list;
	t_ast		*right;

	list = ast_list_new(left);
	if (!list)
		return (ast_free(left), NULL);
	while (*tok && (*tok)->type == TOKEN_PIPE)
	{
		*tok = (*tok)->next;
		if (!*tok || ((*tok)->type != TOKEN_WORD
				&& (*tok)->type != TOKEN_LPAREN
				&& (*tok)->type != TOKEN_REDIR_IN
				&& (*tok)->type != TOKEN_REDIR_OUT
				&& (*tok)->type != TOKEN_REDIR_APPEND
				&& (*tok)->type != TOKEN_HEREDOC))
		{
			set_parser_error(ctx, "expected cmd after pipe", *tok);
			return (ast_list_free(list), NULL);
		}
		right = parser_command_or_subshell(tok, ctx);
		if (ctx->error_status)
			return (ast_list_free(list), NULL);
		list = ast_list_append(list, right);
	}
	return (ast_new_pipe(list));
}
