/*
---> RECOMENDED HELPERS

AST List Management (for N-ary nodes)
----------------------------------------
t_ast_list *ast_list_new(t_ast *node); ✅
Creates a new list node.

t_ast_list *ast_list_append(t_ast_list *head, t_ast *node); ✅
Appends a node to the end of a children list.

void ast_list_free(t_ast_list *list); ✅
Recursively frees an AST children list.

Command Argument Utilities
-----------------------------------------
int ast_count_args(char **argv);          ✅
Returns argument count for a command's argv.

Memory Management
-----------------------------------------
void ast_free(t_ast *node);             ✅
Recursively frees an entire AST and its children.

Debugging / Printing ASTs
-----------------------------------------
void ast_print(const t_ast *root, int indent);
Prints the AST structure for debugging (indentation shows depth).
(You might want optional colored output for clarity.)

Error Handling and Recovery
------------------------------------------
void ast_parser_error(const char *msg, t_token *tok);
Standardizes parser error messages and possibly halts parsing.


???????????????????????????????????????????????????
Token Utilities (in Parser)
------------------------------
t_token *token_next_word(t_token *tok);
Skips non-word tokens, returns next word token.

t_token *token_skip_until(t_token *tok, t_token_type type);
Skips tokens until it finds a desired type (e.g., pipe, redirect).
(Speeds up command grouping in parser logic.)
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

// count childrens
int ast_count_args(char **argv)
{
    int count;

    count = 0;
    while (argv[count])
        count++;
    return (count);

}

// Memory management & free
// Recursively frees an AST children list.
void ast_list_free(t_ast_list *list)
{
	if (!list)
		return ;
	ast_free(list->node);
	ast_list_free(list->next);
	free(list);
}

// free ast_nodes & children
void ast_free(t_ast *node)
{
	if (!node)
		return ;
	ast_free(node->left);
	ast_free(node->right);
	ast_list_free(node->children);
	free(node->argv);
	free(node->file);
	free(node);
}



