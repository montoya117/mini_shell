

#include "nanoshell.h"

t_ast   *parser(t_token **ptokens)
{
    return (parser_logical(ptokens));
}

