/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:57:52 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 11:20:00 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
#include <stdio.h>

int	main()
{
	char	*str1 = "42 to the Moon!";
	t_list	*nodo1;

	nodo1 = ft_lstnew(str1);
	if (!nodo1)
	{
		printf("Error al crear el nodo!");
		return (1);
	}

	printf("El contenido es : %s\n", (char *)nodo1->content);
	printf("El next de este nodo es: %p\n", (void *) nodo1->next);
	free(nodo1);
	return (0);
}*/
