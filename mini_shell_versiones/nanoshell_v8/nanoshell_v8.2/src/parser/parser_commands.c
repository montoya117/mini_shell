
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
    t_token         *last_word;
    t_token         *words_start;
    t_token         *tmp;
    t_token_type    redirect_type;
    char            *redirect_file;
    int             fd_dest;
    t_ast           *cmd;
    t_ast           *cmd_node;
	
	token = *ptokens;
    // Reset io_number antes de construir argv
    tmp = token;
    while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
    {
        if (tmp->type == TOKEN_WORD)
            tmp->is_io_number = 0;
        tmp = tmp->next;
    }
	// 1- check for redirections without prvious command
	if (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
	{
		set_parser_error(ctx,  "redirection without command", token);
		return (NULL);
	}
	// 2- PRIMERA PASADA: contar assign + args ignorando io_number
    assign_count = 0;
    argc = 0;
    // 2a) contar assignments iniciales
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION)
        && is_assignment_word(token->text))
    {
        assign_count++;
        token = token->next;
    }
    // 2b) contar palabras de argv (de momento todas)
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
    // 4- SEGUNDA PASADA: assignments + preparar last_word y redirecciones
    // 4a) assignments
    token = *ptokens;
    i = 0;
    while (i < assign_count)
    {
        assignments[i] = ft_strdup(token->text);
        i++;
        token = token->next;
    }
    if (assignments)
        assignments[assign_count] = NULL;
    // guardar inicio de las palabras para la tercera pasada
    words_start = token;
    // 4b) avanzar por las palabras (sin copiar aún a argv), guardando last_word
    last_word = NULL;
    while (token && (token->type == TOKEN_WORD || token->type == TOKEN_EXPANSION))
    {
        if (token->type == TOKEN_WORD)
            last_word = token;
        token = token->next;
    }
    // aquí token está en la primera redirección (o NULL)
    // crear nodo comando ahora, redirecciones se irán encadenando a partir de él
    cmd_node = ast_new_command(NULL, assignments); // luego pondrás argv
    cmd = cmd_node; // cmd se usará para encadenar redirects
    // 5- procesar redirecciones y marcar io_number + redirect_fd
    while (token && (token->type == TOKEN_REDIR_IN
        || token->type == TOKEN_REDIR_OUT 
        || token->type == TOKEN_REDIR_APPEND
        || token->type == TOKEN_HEREDOC))
    {
        redirect_type = token->type;
        fd_dest = -1;   // !! importante
        //  Detectar io_number sólo para > y >> si cumple condiciones
        if ((redirect_type == TOKEN_REDIR_OUT
             || redirect_type == TOKEN_REDIR_APPEND)
            && last_word
            && last_word->type == TOKEN_WORD
            && is_all_digits(last_word->text)
            && last_word->join_next
            && !last_word->was_quoted)
        {
            fd_dest = ft_atoi(last_word->text);
            last_word->is_io_number = 1;    // marcar para saltarlo en argv
        }
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
            if (fd_dest == -1)
                fd_dest = 0;        // stdin para heredoc
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
            if (fd_dest == -1)
            {
                if (redirect_type == TOKEN_REDIR_IN)
                    fd_dest = 0;
                else
                    fd_dest = 1;
            }
        }
        cmd = ast_new_redirect(cmd, redirect_file, redirect_type);
        cmd->redirect_fd = fd_dest;
        token = token->next;
    }
    // 6- TERCERA PASADA: llenar argv saltando io_number
    i = 0;
    tmp = words_start;
    while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
    {
        if (tmp->type == TOKEN_WORD && !tmp->is_io_number)
        {
            argv[i] = ft_strdup(tmp->text);
            i++;
        }
        else if (tmp->type == TOKEN_EXPANSION)
        {
            argv[i] = ft_strdup(tmp->text);  // expansions NO son io_number
            i++;
        }
        tmp = tmp->next;
    }
    argv[i] = NULL;
    cmd_node->argv = argv;
    // IMPORTANTE: aquí la posición correcta para el parser es donde la dejó el bucle de redirecciones
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
        // Parsear el interior con la misma regla que usa parser(), pero
        // SIN hacer el check de "no queden tokens"
        subshell = parser_logical(&token, ctx);
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
