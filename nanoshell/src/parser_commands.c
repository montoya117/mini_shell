
#include "nanoshell.h"

// parser_commands--->
// 1. Gather command and arguments
// 2. While more tokens, if redirection: attach node, else break
// 3. Handle subshells
// 4. Return root command node

/*
    token = *ptokens;
i = 0;
while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
{
    if (token->type == TOKEN_WORD) {
        argv[i] = token->text;
    } else if (token->type == TOKEN_EXPANSION) {
        t_ast *expnode = ast_new_expansion(token->text);
        cmd = ast_attach_expansion(cmd, expnode); // Attach to the command node
        argv[i] = token->text; // Optionally, store for argv evaluation phase
    }
    i++;
    token = token->next;
}
argv[i] = NULL;
cmd = ast_new_command(argv);

*/

t_ast *parser_commands(t_token **ptokens)
{
    int             argc;
    int             i;
    char            **argv;
    t_token         *token;
    t_token_type    redirect_type;
    t_ast           *cmd;
    t_ast           *subshell;
    t_ast           *expansion_node;

    token = *ptokens;
    argc = 0;
    // count words and expansions
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
    {
        argc++;
        token = token->next;
    }
    // collect and malloc arguments
    argv = safe_malloc((argc + 1) * sizeof(char*));
    token = *ptokens;
    i = 0;
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
    {
        argv[i] = token->text;
        i++;
        token = token->next;
    }
    argv[i] = NULL;
    cmd = ast_new_command(argv);
    
    //  if more tokens, look for rediretions 
    while (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT 
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
    {
        redirect_type = token->type;
        token = token->next;
        // make sure it comes a file after the redirection if not handle
        if (!token || token->type != TOKEN_WORD)
        {
            ast_parser_error("missing file name after redirection", token);
            return (NULL);
        }
        cmd = ast_new_redirect(cmd, token->text, redirect_type);
        token = token->next;
    }
    // Check for subshells
    if (token && token->type == TOKEN_LPAREN)
    {
        token = token->next; // Skip the '('
        subshell = parser(&token);
        if (!token || token->type != TOKEN_RPAREN)
        {
            ast_parser_error( "missing the ')'", token);
            return (NULL);
        }
        token = token->next; // Skip the ')'
        cmd = ast_new_subshell(subshell);
    }
    *ptokens = token;
    return (cmd);
}