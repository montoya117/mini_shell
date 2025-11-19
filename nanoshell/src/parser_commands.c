
#include "nanoshell.h"

// parser_commands--->
// 1. Gather command and arguments
// 2. While more tokens, if redirection: attach node, else break
// 3. Handle subshells
// 4. Return root command node

t_ast *parser_commands(t_token **ptokens)
{
    int             argc;
    int             i;
    char            **argv;
    t_token         *token;
    t_token_type    redirect_type;
    t_ast           *cmd;
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
    *ptokens = token;
    return (cmd);
}

t_ast	*parser_logical(t_token **ptokens)
{
	t_token			*token;
	t_token_type	logic_type;
	t_ast			*left;
	t_ast			*right;

    left = parser_pipe(ptokens);
	token = *ptokens;
	while (token && (token->type == TOKEN_AND || token->type == TOKEN_OR))
    {
        logic_type = token->type;
    
        token = token->next;
        right = parser_pipe(&token);
        if (logic_type == TOKEN_AND)
            left = ast_new_and(left, right);
        else if (logic_type == TOKEN_OR)
            left = ast_new_or(left, right);
    }
    *ptokens = token;
    return (left);
}


t_ast   *parser_pipe(t_token **ptokens)
{
    t_token			*token;
    t_ast           *pipe_node;
	t_ast			*left;
	t_ast			*right;
    t_ast_list      *pipe_list;

    left = parser_commnand_or_subshell(ptokens);
    token = *ptokens;
    pipe_list = ast_list_new(left);
    while (token && token->type == TOKEN_PIPE)
    {
        token = token->next;
        right  = parser_command_or_subshell(&token);
        // create new list with left, add right.. create the ast_pipe with this
        pipe_list = ast_list_append(pipe_list, right);
    }
    *ptokens = token;
    pipe_node = ast_new_pipe(pipe_list);
    return (pipe_node);
}

t_ast   *parser_subshell(t_token **ptokens)
{
    t_token *token;
    t_ast   *subshell;
    t_ast   *cmd;
    
    token = *ptokens;
    cmd = NULL;
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

t_ast   *parse_command_or_subshel(t_token **ptokens)
{
    if (*ptokens && (*ptokens)->type == TOKEN_LPAREN)
        return parse_subshell(ptokens);
    else
        return parser_commands(ptokens);
}