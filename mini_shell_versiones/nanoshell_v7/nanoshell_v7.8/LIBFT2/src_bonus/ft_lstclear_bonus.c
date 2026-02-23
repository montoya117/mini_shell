/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:01:05 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 12:46:50 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	if (!lst || !*lst || !del)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = tmp;
	}
	*lst = NULL;
}
/*
#include <stdio.h>

#include <stdio.h>

void	del(void *content)
{
	free(content);
}

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
	ft_lstclear(&lista, del);
	return (0);
}*/
