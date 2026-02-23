/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:47:29 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 13:01:29 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	i;

	i = 0;
	while (lst)
	{
		i++;
		lst = lst->next;
	}
	return ((int)i);
}
/*
#include <stdio.h>

int	main()
{
	char	*str1 = ft_strdup("42 ");
	char	*str2 = ft_strdup("to the Moon!");
	t_list	*lista = NULL;
	t_list	*nodo1 = ft_lstnew(str1);
	t_list	*nodo2 = ft_lstnew(str2);
	if (!nodo1 || !nodo2)
		return (1);
	ft_lstadd_back(&lista, nodo1);
	ft_lstadd_back(&lista, nodo2);
	t_list	*temp = lista;
	while (temp)
	{	
		printf("%s\n", (char *) temp->content);
		temp = temp->next;
	}
	printf("%d\n" ,ft_lstsize(lista));
	return (0);
}*/
