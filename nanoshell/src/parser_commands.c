
#include "nanoshell.h"

t_ast *parser_commands(t_token *tokens)
{
    int     argc;
    int     i;
    char    **argv;
    t_token *tok;
    t_ast   *cmd;

    tok = tokens;
    argc = 0;
    // count commands
    while (tok && tok->type == TOKEN_WORD)
    {
        argc++;
        tok = tok->next;
    }
    // collect and malloc arguments
    argv = safe_malloc((argc + 1) * sizeof(char*));
    tok = tokens;
    i = 0;
    while (tok && tok->type == TOKEN_WORD)
    {
        argv[i] = tok->text;
        i++;
        tok = tok->next;
    }
    argv[i] = NULL;
    cmd = ast_new_command(argv);
    return (cmd);
}