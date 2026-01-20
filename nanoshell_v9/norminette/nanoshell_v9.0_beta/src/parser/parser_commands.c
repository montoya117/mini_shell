
#include "nanoshell.h"

// Check ctx->error_status after each parsing step, and early-return if an error is detected.

t_ast *parser_commands(t_token **ptokens, t_parser_context *ctx)
{
	t_token     *token;
	t_token     *tmp;
	t_token     *last_word;
	t_token     *words_start;
	t_ast       *cmd;
	t_ast       *base_cmd;
	t_token_type redirect_type;
	char        *redirect_file;
	int         fd_dest;
	int         assign_count;
	int         argc;
	int         i;
	char        **assignments;
	char        **argv;

	token = *ptokens;

	/* 0- crear comando base vacío (para soportar redirecciones iniciales) */
	cmd = ast_new_command(NULL, NULL);
	if (!cmd)
		return (NULL);

	/* 1- REDIRECCIONES INICIALES: <in >out </dev/null ... */
	while (token && (token->type == TOKEN_REDIR_IN
			|| token->type == TOKEN_REDIR_OUT
			|| token->type == TOKEN_REDIR_APPEND
			|| token->type == TOKEN_HEREDOC))
	{
		redirect_type = token->type;
		fd_dest = -1;
		token = token->next;
		if (!token || token->type != TOKEN_WORD)
		{
			set_parser_error(ctx, "missing file name after redirection", token);
			ast_free(cmd);
			return (NULL);
		}
		redirect_file = NULL;
		if (redirect_type == TOKEN_HEREDOC)
		{
			redirect_file = create_heredoc_tmp(token->text);
			if (!redirect_file)
			{
				set_parser_error(ctx, "heredoc creation failed", token);
				ast_free(cmd);
				return (NULL);
			}
			if (fd_dest == -1)
				fd_dest = 0;
		}
		else
		{
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

	/* 2- PRIMERA PASADA: contar assignments + args (WORD / EXPANSION) */
	assign_count = 0;
	argc = 0;

	tmp = token;
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION)
		&& is_assignment_word(tmp->text))
	{
		assign_count++;
		tmp = tmp->next;
	}
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
	{
		argc++;
		tmp = tmp->next;
	}

	if (assign_count == 0 && argc == 0)
	{
		/* solo redirecciones (tipo </dev/null >/tmp/log) → decide si lo aceptas o lo tratas como error */
		if (!cmd || cmd->type != AST_REDIRECT)
		{
			set_parser_error(ctx, "empty command before operator", tmp);
			ast_free(cmd);
			return (NULL);
		}
		*ptokens = tmp;
		return (cmd);
	}

	/* 3- reservar arrays */
	assignments = NULL;
	if (assign_count > 0)
		assignments = safe_malloc((assign_count + 1) * sizeof(char *));
	argv = safe_malloc((argc + 1) * sizeof(char *));

	/* 4- SEGUNDA PASADA: copiar assignments y preparar last_word/io_number */
	i = 0;
	while (i < assign_count)
	{
		assignments[i] = ft_strdup(token->text);
		i++;
		token = token->next;
	}
	if (assignments)
		assignments[assign_count] = NULL;

	words_start = token;

	last_word = NULL;
	tmp = token;
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
	{
		if (tmp->type == TOKEN_WORD)
			last_word = tmp;
		tmp = tmp->next;
	}
	/* tmp ahora está en la primera redirección final (o NULL) */

	/* 5- REDIRECCIONES FINALES + io_number */
	token = tmp;
	while (token && (token->type == TOKEN_REDIR_IN
			|| token->type == TOKEN_REDIR_OUT
			|| token->type == TOKEN_REDIR_APPEND
			|| token->type == TOKEN_HEREDOC))
	{
		redirect_type = token->type;
		fd_dest = -1;

		if ((redirect_type == TOKEN_REDIR_OUT
			 || redirect_type == TOKEN_REDIR_APPEND)
			&& last_word
			&& last_word->type == TOKEN_WORD
			&& is_all_digits(last_word->text)
			&& last_word->join_next
			&& !last_word->was_quoted)
		{
			fd_dest = ft_atoi(last_word->text);
			last_word->is_io_number = 1;
		}

		token = token->next;
		if (!token || token->type != TOKEN_WORD)
		{
			set_parser_error(ctx, "missing file name after redirection", token);
			ast_free(cmd);
			return (NULL);
		}

		redirect_file = NULL;
		if (redirect_type == TOKEN_HEREDOC)
		{
			redirect_file = create_heredoc_tmp(token->text);
			if (!redirect_file)
			{
				set_parser_error(ctx, "heredoc creation failed", token);
				ast_free(cmd);
				return (NULL);
			}
			if (fd_dest == -1)
				fd_dest = 0;
		}
		else
		{
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

	/* 6- TERCERA PASADA: llenar argv saltando io_number */
	i = 0;
	tmp = words_start;
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
	{
		if (tmp->type == TOKEN_WORD && !tmp->is_io_number)
			argv[i++] = ft_strdup(tmp->text);
		else if (tmp->type == TOKEN_EXPANSION)
			argv[i++] = ft_strdup(tmp->text);
		tmp = tmp->next;
	}
	argv[i] = NULL;

	/* 7- asignar argv/assignments al AST_COMMAND de la base */
	base_cmd = cmd;
	while (base_cmd && base_cmd->type == AST_REDIRECT)
		base_cmd = base_cmd->left;

	if (!base_cmd || base_cmd->type != AST_COMMAND)
	{
		set_parser_error(ctx, "internal parser error", token);
		ast_free(cmd);
		return (NULL);
	}
	base_cmd->argv = argv;
	base_cmd->assignments = assignments;

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
