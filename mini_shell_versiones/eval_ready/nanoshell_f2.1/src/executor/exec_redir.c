/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:18:22 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:18:25 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static t_ast	*collect_redirs(t_ast *node, t_ast ***reds, int *n)
{
	t_ast	*tmp;
	int		i;

	tmp = node;
	*n = 0;
	while (tmp && tmp->type == AST_REDIRECT)
	{
		(*n)++;
		tmp = tmp->left;
	}
	if (*n == 0)
		return (tmp);
	*reds = safe_malloc((*n) * sizeof(t_ast *));
	tmp = node;
	i = 0;
	while (i < *n)
	{
		(*reds)[i++] = tmp;
		tmp = tmp->left;
	}
	return (tmp);
}

void	apply_redirect_and_exec_child(t_ast *node, t_data *data)
{
	t_ast	*child;
	t_ast	**reds;
	int		n;

	if (!node)
		exit(1);
	n = 0;
	reds = NULL;
	child = collect_redirs(node, &reds, &n);
	if (reds)
	{
		if (execute_redirection_list(reds, n) == -1)
		{
			free(reds);
			exit(1);
		}
		free(reds);
	}
	if (!child)
		exit(0);
	if (child->type == AST_COMMAND)
		exec_child(child, data);
	else
		exec_ast_in_child(child, data);
	exit(127);
}

int	apply_all_redirections(t_ast *node)
{
	t_ast	**reds;
	int		n;
	int		status;

	reds = NULL;
	n = 0;
	collect_redirs(node, &reds, &n);
	if (!reds)
		return (0);
	status = execute_redirection_list(reds, n);
	free(reds);
	return (status);
}

void	unlink_heredocs(t_ast *node)
{
	while (node && node->type == AST_REDIRECT)
	{
		if (node->redirect_type == TOKEN_HEREDOC && node->file)
			unlink(node->file);
		node = node->left;
	}
}
