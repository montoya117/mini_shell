/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer_utils_helper.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 15:51:26 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/12 15:09:49 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

void	buf_init(t_buf *b)
{
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}

void	buf_free(t_buf *b)
{
	if (!b)
		return ;
	free(b->data);
	b->data = NULL;
	b->len = 0;
	b->cap = 0;
}
