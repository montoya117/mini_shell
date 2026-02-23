/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 10:44:24 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 12:38:05 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
/*
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
	ft_lstdelone(nodo1, del);
	return (0);
}*/
