/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:54:31 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 15:25:45 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int  exec_or(t_ast *node, t_data *data)
{
    int status;

    if (!node || node->type != AST_OR)
        return (1);
    status = exec_ast(node->left, data);
    if ( status != 0)
        return (exec_ast(node->right, data));
    return (status);    
}

static int  exec_and(t_ast *node, t_data *data)
{
    int status;

        if (!node || node->type != AST_AND)
            return (1);
        status = exec_ast(node->left, data);
        if  (status == 0)
            return (exec_ast(node->right, data));
        return (status);
}

int exec_ast_in_child(t_ast *node, t_data *data)
{
    int status;

    if (!node)
        return (0);
    else if (node->type == AST_COMMAND)
    {
        exec_child(node, data); // never returns on success
        return (127); // fallback
    }
    else if (node->type == AST_AND)
        status = exec_and(node, data);
    else if (node->type == AST_OR)
        status = exec_or(node, data);
    else if (node->type == AST_PIPE)
        status = exec_pipeline(node, data);
    else if (node->type == AST_SUBSHELL)
        status = exec_subshell(node, data);
    else
        status = 1; // no AST_REDIRECT here on purpose
    return status;
}

int	exec_ast(t_ast *node, t_data *data)
{
	int	status;

	if (!node)
		return (0);
	else if (node->type == AST_COMMAND)
		status = run_single(node, data);
	else if (node->type == AST_AND)
		status = exec_and(node, data);
	else if (node->type == AST_OR)
		status = exec_or(node, data);
	else if (node->type == AST_PIPE)
		status = exec_pipeline(node, data);
	else if (node->type == AST_REDIRECT)
		status = apply_redirect_and_exec(node, data);
	else if (node->type == AST_SUBSHELL)
		return (exec_subshell(node, data));
	else
		status = 1;
	data->last_status = status;
	return (status);
}
