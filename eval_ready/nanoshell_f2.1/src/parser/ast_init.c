/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:20:11 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:20:13 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// Logic AND/OR node constructor
t_ast	*ast_new_and(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_AND;
	node->left = left;
	node->right = right;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	node->argv_quoted = NULL;
	return (node);
}

t_ast	*ast_new_or(t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = safe_malloc(sizeof(t_ast));
	node->type = AST_OR;
	node->left = left;
	node->right = right;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	node->argv_quoted = NULL;
	return (node);
}

// Subshell node constructor
t_ast	*ast_new_subshell(t_ast *child)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	node->type = AST_SUBSHELL;
	node->left = child;
	node->right = NULL;
	node->argv = NULL;
	node->assignments = NULL;
	node->file = NULL;
	node->redirect_type = 0;
	node->redirect_fd = -1;
	node->children = NULL;
	node->argv_quoted = NULL;
	return (node);
}
