/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 09:38:52 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 13:24:25 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
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
	return (0);
}*/
