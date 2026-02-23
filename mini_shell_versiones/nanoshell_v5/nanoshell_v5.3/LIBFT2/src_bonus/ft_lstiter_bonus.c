/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 11:45:37 by jadelgad          #+#    #+#             */
/*   Updated: 2025/05/21 13:22:46 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*
#include <stdio.h>

void	to_upper(void *content)
{
	char	*str = (char *)content;
	int	i = 0;
	while (str[i])
	{
		str[i] = ft_toupper(str[i]);
		i++;
	}
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
	ft_lstiter(lista, to_upper;
	temp = lista;
	while (temp)
	{
		printf("%s\n", (char *)temp->content);
		temp = temp->next;
	}
	return (0);
}*/
