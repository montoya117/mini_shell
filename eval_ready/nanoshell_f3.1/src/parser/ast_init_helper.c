/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:20:11 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:20:13 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// Command node constructor
t_ast	*ast_new_command(char **tokens, char **assignments)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_COMMAND;
	node->left = NULL;
	node->right = NULL;
	node->argv = tokens;
	node->assignments = assignments;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	node->argv_quoted = NULL;
	return (node);
}

// Pipe node constructor
t_ast	*ast_new_pipe(t_ast_list *children)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_PIPE;
	node->left = NULL;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = children;
	node->argv_quoted = NULL;
	return (node);
}

// Redirect node constructor
t_ast	*ast_new_redirect(t_ast *cmd, char *file, int redirect_type)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_REDIRECT;
	node->left = cmd;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = file;
	node->redirect_type = redirect_type;
	node->children = NULL;
	node->argv_quoted = NULL;
	if (redirect_type == TOKEN_REDIR_IN || redirect_type == TOKEN_HEREDOC)
		node->redirect_fd = 0;
	else
		node->redirect_fd = 1;
	return (node);
}
