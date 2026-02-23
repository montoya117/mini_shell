
#include "nanoshell.h"

// Check ctx->error_status after each parsing step, and early-return if an error is detected.

t_ast *parser_commands(t_token **ptokens, t_parser_context *ctx)
{
    int             argc;
    int             assign_count;
    int             i;
    char            **argv;
    char            **assignments;
    t_token         *token;
    t_token_type    redirect_type;
    char            *redirect_file;
    t_ast           *cmd;
	
	token = *ptokens;
	// 1- check for redirections without prvious command
	if (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
	{
		set_parser_error(ctx,  "redirection without command", token);
		return (NULL);
	}
	// 2- FIRST PASS: count assignments + normal args
    assign_count = 0;
    argc = 0;
    // count leading assignment words
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION)
        && is_assignment_word(token->text))
    {
        assign_count++;
        token = token->next;
    }
    // count remaining words as normal argv
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
    {
        argc++;
        token = token->next;
    }
	if (assign_count == 0 && argc == 0) 
	{
		set_parser_error(ctx, "empty command before operator", token);
		return NULL;
	}
    // 3- allocate arrays
    assignments = NULL;
    if (assign_count > 0)
        assignments = safe_malloc((assign_count + 1) * sizeof(char *));
    argv = safe_malloc((argc + 1) * sizeof(char *));
    // 4- SECOND PASS: fill arrays
    token = *ptokens;
    // if assign_count = 0  will skip to argv
    i = 0;
    while (i < assign_count)
    {
        assignments[i] = ft_strdup(token->text);
        i++;
        token = token->next;
    }
    if (assignments)
        assignments[assign_count] = NULL;
    i = 0;
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
    {
        argv[i] = ft_strdup(token->text);
        i++;
        token = token->next;
    }
    argv[i] = NULL;
    // 5- build command node with assignments
    cmd = ast_new_command(argv, assignments);
    
    // 6- if more tokens
    // parse and wrap redirections
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
            ast_free(cmd);
            return (NULL);
        }
        // heredoc
        redirect_file = NULL;
        if (redirect_type == TOKEN_HEREDOC)
        {
            // token->text is the delimeter
            redirect_file = create_heredoc_tmp(token->text);
            if (!redirect_file)
            {
                // error
                set_parser_error(ctx, "heredoc creation failed", token);
                ast_free(cmd);
                return (NULL);
            }
        }
        else
        {
            // normal redirection
            redirect_file = ft_strdup(token->text);
            if (!redirect_file)
            {
                set_parser_error(ctx, "memory allocation failed", token);
                ast_free(cmd);
                return (NULL);
            }
        }
        cmd = ast_new_redirect(cmd, redirect_file, redirect_type);
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

	token = *ptokens;
	if (token && (token->type == TOKEN_AND || token->type == TOKEN_OR)) 
	{
    	set_parser_error(ctx, "operator at start of input", token);
    	return NULL;
	}
    left = parser_pipe(&token, ctx);
	if (ctx->error_status)
		return (NULL);
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
	
	token = *ptokens;
	if (token && token->type == TOKEN_PIPE)
	{
		set_parser_error(ctx, "pipe at start of input", token);
		return NULL;
	}
    left = parser_command_or_subshell(&token, ctx);
	if (ctx->error_status)
        return (NULL);
	// / No pipe: just return the node we got (AST_COMMAND, AST_REDIRECT, AST_SUBSHELL)
	if (!token || token->type != TOKEN_PIPE)
	{
		*ptokens = token;
		return (left);
	}
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
