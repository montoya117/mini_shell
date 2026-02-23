/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:42:21 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 11:31:12 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	new -> next = *lst;
	*lst = new;
}
/*
#include <stdio.h>

int	main()
{
	char	*str1 = "42 to ";
	char	*str2 = "the Moon!";
	t_list	nodo1;
	t_list	nodo2;
	t_list	lista = NULL;

	nodo1 = ft_lstnew(str1);
	nodo2 = ft_lstnew(str2);
	if (!nodo1 || !nodo2)
		return (1);

	ft_lstadd_front(&lista, nodo2);
	ft_lstadd_front(&lista, nodo1);
	while (lista)
	{
		printf("Conrtenido de nodo: %s\n", (char *)lista->content);
		lista = lista->next;
	}
	return (0);
}*/
