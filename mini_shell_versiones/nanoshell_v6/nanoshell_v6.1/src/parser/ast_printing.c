/*
Debugging / Printing ASTs
-----------------------------------------
void ast_print(const t_ast *root, int indent);
Prints the AST structure for debugging (indentation shows depth).
(You might want optional colored output for clarity.)
*/

#include "nanoshell.h"

// Debugging / Printing ASTs & Tokens
const char *ast_to_string(const AST_TYPE type)
{
    if (type == AST_COMMAND)
        return (C"Command"RST);
    else if (type == AST_PIPE)
        return (G"Pipe"RST);
    else if (type == AST_REDIRECT)
        return (RED"Redirection"RST);
    else if (type == AST_AND)
        return (Y"And"RST);
    else if (type == AST_OR)
        return (Y"Or"RST);
    else if (type == AST_SUBSHELL)
        return (M"Subshell"RST);
    else
        return ("Unknown");
}

void ast_print(const t_ast *root, int indent)
{
	int         i;
    int         j;
    int         k;
	t_ast_list  *current; 

	if (!root)
		return ;
	// print indetation space
	i = 0;
	while (i < indent)
	{
		printf(" ");
		i++;
	}
	// print node info
    printf("AST TYPE --> %s\n", ast_to_string(root->type));
	if (root->type == AST_COMMAND && root->argv)
    {
        j = 0;
        while (root->argv[j])
        {
            k = 0;
            while ( k < indent + 2)
            {
                printf(" ");
                k++;
            }
            printf("argv[%d]: %s\n", j, root->argv[j]);
            j++;
        }
    }
    // Print left/right children deopending on type
    ast_print(root->left, indent + 2);
    ast_print(root->right, indent + 2);
	// Print list of children depending on type
    current = root->children;
    while(current)
    {
        ast_print(current->node, indent + 2);
        current =  current->next;
    }
}