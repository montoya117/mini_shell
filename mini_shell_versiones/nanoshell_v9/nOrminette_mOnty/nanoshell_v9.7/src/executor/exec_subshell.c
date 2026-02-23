/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:18:33 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/04 15:18:35 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

int	exec_subshell(t_ast *node, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!node || node->type != AST_SUBSHELL || !node->left)
		return (1);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		reset_sig_in_child();
		status = exec_ast(node->left, data);
		exit(status);
	}
	return (wait_for_child(pid));
}
