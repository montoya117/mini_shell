/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:21:05 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:21:07 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

// AST List Management
// Creates a new list node.
t_ast_list	*ast_list_new(t_ast *node)
{
	t_ast_list	*list;

	list = safe_malloc(sizeof(t_ast_list));
	list->node = node;
	list->next = NULL;
	return (list);
}

// Appends a node to the end of a children list.
t_ast_list	*ast_list_append(t_ast_list *head, t_ast *node)
{
	t_ast_list	*tmp;
	t_ast_list	*new_node;

	if (!node)
		return (head);
	new_node = ast_list_new(node);
	if (!head)
		return (new_node);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (head);
}

// for commands
int	ast_count_args(char **argv)
{
	int	count;

	count = 0;
	while (argv[count])
		count++;
	return (count);
}
