
#include "nanoshell.h"
/*
// Utility: error handling wrapper
void set_parser_error(t_parser_context *ctx, const char *message, t_token *token)
{
    ctx->error_status = 1;
    ctx->message = message;
    ctx->error_token = token;
    ast_parser_error(message, token);
} ✅

// Example in parser_commands:
t_ast *parser_commands(t_token **ptokens, t_parser_context *ctx)
{
    ...
    // Check for illegal token at start (e.g., redirection with no command)
    if (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
    {
        set_parser_error(ctx, "redirection without command", token);
        return NULL;
    }✅
    ...
    // Later, on missing filename after redirection:
    if (!token || token->type != TOKEN_WORD)
    {
        set_parser_error(ctx, "missing file name after redirection", token);
        return NULL;
    }✅
    ...
    // At end: check for garbage tokens after valid command
    if (token && token->type != TOKEN_PIPE && / no more expected tokens /)
    {
        set_parser_error(ctx, "unexpected token after command", token);
        return NULL;
    }
    ...
}

// Example in parser_pipe:
t_ast *parser_pipe(t_token **ptokens, t_parser_context *ctx)
{
    ...
    while (token && token->type == TOKEN_PIPE)
    {
        token = token->next;
        if (!token || (token->type != TOKEN_WORD && token->type != TOKEN_LPAREN))
        {
            set_parser_error(ctx, "expected command after pipe", token);
            return NULL;
        }
        ...
    }
    ...
} ✅

// Example in parser_logical:
t_ast *parser_logical(t_token **ptokens, t_parser_context *ctx)
{
    ...
    while (token && (token->type == TOKEN_AND || token->type == TOKEN_OR))
    {
        token = token->next;
        if (!token)
        {
            set_parser_error(ctx, "operator at end of input", token);
            return NULL;
        }
        ...
    }
    ...
} ✅

// At very top-level, handle end-of-input garbage
t_ast *parser(t_token **ptokens, t_parser_context *ctx)
{
    t_ast *tree = parser_logical(ptokens, ctx);
    if (ctx->error_status)
        return NULL;
    if (*ptokens != NULL)
    {
        set_parser_error(ctx, "unexpected extra input", *ptokens);
        return NULL;
    }
    return tree;
}

*/
// Check ctx->error_status after each parsing step, and early-return if an error is detected.

t_ast *parser_commands(t_token **ptokens, t_parser_context *ctx)
{
    int             argc;
    int             i;
    char            **argv;
    t_token         *token;
    t_token_type    redirect_type;
    t_ast           *cmd;
	
	token = *ptokens;
	// check for redirections without prvious command
	if (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
	{
		set_parser_error(ctx,  "redirection, without command", token);
		return (NULL);
	}
	// count words and expansions
    argc = 0;
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
			set_parser_error(ctx, "missing file name after redirection", token);
            return (NULL);
        }
        cmd = ast_new_redirect(cmd, token->text, redirect_type);
        token = token->next;
    }
    *ptokens = token;
    return (cmd);
}

t_ast	*parser_logical(t_token **ptokens, t_parser_context *ctx)
{
	t_token			*token;
	t_token_type	logic_type;
	t_ast			*left;
	t_ast			*right;

    left = parser_pipe(ptokens, ctx);
	if (ctx->error_status)
		return (NULL);
	token = *ptokens;
	while (token && (token->type == TOKEN_AND || token->type == TOKEN_OR))
    {
        logic_type = token->type;
        token = token->next;
		if (!token)
		{
			set_parser_error(ctx,  "operator at end of input", token);
			return (NULL);
		}
        right = parser_pipe(&token, ctx);
		if (ctx->error_status)
			return (NULL);
        if (logic_type == TOKEN_AND)
            left = ast_new_and(left, right);
        else if (logic_type == TOKEN_OR)
            left = ast_new_or(left, right);
    }
    *ptokens = token;
    return (left);
}


t_ast   *parser_pipe(t_token **ptokens, t_parser_context *ctx)
{
    t_token			*token;
    t_ast           *pipe_node;
	t_ast			*left;
	t_ast			*right;
    t_ast_list      *pipe_list;

    left = parser_command_or_subshell(ptokens, ctx);
	if (ctx->error_status)
        return (NULL);
    token = *ptokens;
    pipe_list = ast_list_new(left);
    while (token && token->type == TOKEN_PIPE)
    {
        token = token->next;
		if (!token || (token->type != TOKEN_WORD && token->type != TOKEN_LPAREN))
		{
			set_parser_error(ctx, "expected command after pipe", token);
			return (NULL);
		}
        right  = parser_command_or_subshell(&token, ctx);
		if (ctx->error_status)
			return (NULL);
        // create new list with left, add right.. create the ast_pipe with this
        pipe_list = ast_list_append(pipe_list, right);
    }
    *ptokens = token;
    pipe_node = ast_new_pipe(pipe_list);
    return (pipe_node);
}

t_ast   *parser_subshell(t_token **ptokens, t_parser_context *ctx)
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
		if (ctx->error_status)
            return (NULL);
        if (!token || token->type != TOKEN_RPAREN)
        {
			set_parser_error(ctx, "missing the ')'", token);
            return (NULL);
        }
        token = token->next; // Skip the ')'
        cmd = ast_new_subshell(subshell);
    }
    *ptokens = token;
    return (cmd);
}

t_ast   *parser_command_or_subshell(t_token **ptokens, t_parser_context *ctx)
{
	t_ast	*node;

	node = NULL;
    if (*ptokens && (*ptokens)->type == TOKEN_LPAREN)
		node = parser_subshell(ptokens, ctx);
    else
		node = parser_commands(ptokens, ctx);
	
	if (ctx->error_status)
		return (NULL);
	return (node);
}
