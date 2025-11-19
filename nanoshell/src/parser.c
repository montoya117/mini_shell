

#include "nanoshell.h"

t_ast   *parser(t_token **ptokens)
{
    t_ast   *root;
    root = parser_logical(ptokens);
    ast_print(root, 0);
    return (root);
}

