
/*
// Assumes 'tok' is the start of your token list
while (tok) {
    if (tok->type == TOKEN_WORD) {
        // Handle words: gather into argv array for commands
        // e.g., add tok->text to current command's argument list
    } 
    else if (tok->type == TOKEN_PIPE) {
        // Handle PIPE: finish current command, create AST_PIPE node
        // e.g., set up left/right children for piping
    }
    else if (
        tok->type == TOKEN_REDIR_IN ||
        tok->type == TOKEN_REDIR_OUT ||
        tok->type == TOKEN_REDIR_APPEND ||
        tok->type == TOKEN_HEREDOC) {
        // Handle I/O redirection: attach filename, set redirect type
    }
    else if (tok->type == TOKEN_AND || tok->type == TOKEN_OR) {
        // Handle logical: create AST_AND/AST_OR nodes
    }
    else if (tok->type == TOKEN_LPAREN) {
        // Handle possible subshell start (advanced)
    }
    else if (tok->type == TOKEN_RPAREN) {
        // Handle possible subshell end (advanced)
    }
    else if (tok->type == TOKEN_EOF) {
        // End of parsing
        break; // or return your AST root
    }
    else if (tok->type == TOKEN_ERROR) {
        // Handle parse error: call ast_parser_error and abort/skip
        ast_parser_error("Unexpected token", tok);
        break;
    }
    // Optionally handle other token types or unexpected tokens here
    tok = tok->next;
}

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

