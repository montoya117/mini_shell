#include "nanoshell.h"

t_piece	*piece_new(char *text, int quoted)
{
	t_piece	*p;

	if (!text)
		return (NULL);
	p = safe_malloc(sizeof(*p));
	p->text = text;
	p->quoted = quoted;
	p->next = NULL;
	return (p);
}

void	piece_append(t_piece **lst, t_piece *node)
{
	t_piece	*cur;

	if (!lst || !node)
		return ;
	if (!*lst)
	{
		*lst = node;
		return ;
	}
	cur = *lst;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

void	piece_free_all(t_piece *lst)
{
	t_piece	*n;

	while (lst)
	{
		n = lst->next;
		free(lst->text);
		free(lst);
		lst = n;
	}
}
