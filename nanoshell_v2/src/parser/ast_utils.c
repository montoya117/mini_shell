/*

AST List Management (for N-ary nodes)
----------------------------------------
t_ast_list *ast_list_new(t_ast *node); ✅
Creates a new list node.

t_ast_list *ast_list_append(t_ast_list *head, t_ast *node); ✅
Appends a node to the end of a children list.

Command Argument Utilities
-----------------------------------------
int ast_count_args(char **argv);          ✅
Returns argument count for a command's argv.

*/

#include "nanoshell.h"

//void ast_list_free(t_ast_list *list);
//void ast_free(t_ast *node);

// AST List Management

// Creates a new list node.
t_ast_list *ast_list_new(t_ast *node)
{
	t_ast_list	*list;

	list = safe_malloc(sizeof(t_ast_list));
	list->node = node;
	list->next = NULL;
	return (list);
}

// Appends a node to the end of a children list.
t_ast_list *ast_list_append(t_ast_list *head, t_ast *node)
{
	t_ast_list	*tmp;
	t_ast_list	*new_node;

	if (!node)
		return (head);
	new_node = ast_list_new(node);
	if (!head)
		return (new_node);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	return (head);
}

// for commands
int ast_count_args(char **argv)
{
    int count;

    count = 0;
    while (argv[count])
        count++;
    return (count);

}



