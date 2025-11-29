/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:55:29 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:55:32 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int  fill_children(t_ast_list *children, t_ast **ar)
{
    int i;

    i = 0;
    if (!ar)
        return (-1);
    while (children)
    {
        ar[i++] = children->node;
        children = children->next;
    }
    return (0);
}

static t_ast    **alloc_children(int n)
{
    if (n <= 0)
        return (NULL);
    return (safe_malloc(sizeof(t_ast *) * n));
}

static int  count_children(t_ast_list *children)
{
    int n;

    n = 0;
    while (children)
    {
        n++;
        children = children->next;
    }
    return (n);
}

static int  collect_children(t_ast_list *children, t_ast ***out_array)
{
    int n;

    if (!out_array)
        return (-1);
    n = count_children(children);
    if (n == 0)
    {
        *out_array = NULL;
        return (0);
    }
    *out_array = alloc_children(n);
    if (!*out_array)
        return (-1);
    if (fill_children(children, *out_array) == -1)
    {
        free(*out_array);
        *out_array = NULL;
        return (-1);
    }
    return (n);
}

int exec_pipeline(t_ast *node, t_data *data)
{
    t_ast   **cmds;
    int     n;
    int     status;

    if (!node || !data || !node->children)
        return (0);

    cmds = NULL;
    status = 0;
    n = collect_children(node->children, &cmds);
    if (n < 0)
        return (127);
    if (n == 0)
        return (0);
    if (n == 1)
        status = run_single(cmds[0], data);
    else
        status = run_multi(cmds, n, data);
    free(cmds);
    return (status);
}
