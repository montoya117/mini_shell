
/*
t_ast *parser(t_token **ptokens)
{
    return parse_logical(ptokens);
}

t_ast *parse_logical(t_token **ptokens)
{
    // Calls parse_pipe() and joins nodes with AST_AND/AST_OR
}

t_ast *parse_pipe(t_token **ptokens)
{
    // Calls parse_simple_command_or_subshell() and joins with AST_PIPE
}

t_ast *parse_simple_command_or_subshell(t_token **ptokens)
{
    if (*ptokens && (*ptokens)->type == TOKEN_LPAREN)
        return parser_subshell(ptokens);
    else
        return parser_commands(ptokens);
}

t_ast *parser_subshell(t_token **ptokens)
{
    // Skips '(', recurses with parser(), asserts ')',
    // returns AST_SUBSHELL node
}


t_ast *parser(t_token **ptokens)
{
    return parse_logical(ptokens);
}

t_ast *parse_logical(t_token **ptokens)
{
    t_ast *left = parse_pipe(ptokens);
    while (*ptokens && ((*ptokens)->type == TOKEN_AND || (*ptokens)->type == TOKEN_OR)) {
        t_token_type op = (*ptokens)->type;
        *ptokens = (*ptokens)->next;
        t_ast *right = parse_pipe(ptokens);
        left = (op == TOKEN_AND) ? ast_new_and(left, right) : ast_new_or(left, right);
    }
    return left;
}

t_ast *parse_pipe(t_token **ptokens)
{
    t_ast *left = parser_commands(ptokens);
    while (*ptokens && (*ptokens)->type == TOKEN_PIPE) {
        *ptokens = (*ptokens)->next;
        t_ast *right = parser_commands(ptokens);
        left = ast_new_pipe(left, right);
    }
    return left;
}

// parser_commands: as you now have it


*/

#include "nanoshell.h"

t_ast   *parser(t_token *tokens)
{
    t_token *tok;

    tok = tokens;
    while (tok)
    {
        if (tok->type == TOKEN_WORD)
        {
            // Handle words: gather from argv array for commands
        }
        // ...more cases as you expand...
        tok = tok->next;
    }
    return (NULL); //TODO
}

