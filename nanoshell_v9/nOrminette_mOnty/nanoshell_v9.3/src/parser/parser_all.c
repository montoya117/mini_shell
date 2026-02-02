#include "nanoshell.h"

static t_ast	*create_logic_node(t_token_type type, t_ast *left, t_ast *right)
{
	if (type == TOKEN_AND)
		return (ast_new_and(left, right));
	return (ast_new_or(left, right));
}

t_ast	*parser_logical(t_token **ptokens, t_parser_context *ctx)
{
	t_token			*token;
	t_token_type	type;
	t_ast			*left;
	t_ast			*right;

	token = *ptokens;
	if (token && (token->type == TOKEN_AND || token->type == TOKEN_OR))
		return (set_parser_error(ctx, "operator at start", token), NULL);
	left = parser_pipe(&token, ctx);
	if (ctx->error_status)
		return (NULL);
	while (token && (token->type == TOKEN_AND || token->type == TOKEN_OR))
	{
		type = token->type;
		token = token->next;
		if (!token)
			return (set_parser_error(ctx, "operator at end", token), NULL);
		right = parser_pipe(&token, ctx);
		if (ctx->error_status)
			return (ast_free(left), NULL);
		left = create_logic_node(type, left, right);
	}
	*ptokens = token;
	return (left);
}

static t_ast	*parse_pipe_sequence(t_ast *left, t_token **tok,
									t_parser_context *ctx)
{
	t_ast_list	*list;
	t_ast		*right;

	list = ast_list_new(left);
	if (!list)
		return (ast_free(left), NULL);
	while (*tok && (*tok)->type == TOKEN_PIPE)
	{
		*tok = (*tok)->next;
		if (!*tok || ((*tok)->type != TOKEN_WORD
				&& (*tok)->type != TOKEN_LPAREN))
		{
			set_parser_error(ctx, "expected cmd after pipe", *tok);
			return (ast_list_free(list), NULL);
		}
		right = parser_command_or_subshell(tok, ctx);
		if (ctx->error_status)
			return (ast_list_free(list), NULL);
		list = ast_list_append(list, right);
	}
	return (ast_new_pipe(list));
}

t_ast	*parser_pipe(t_token **ptokens, t_parser_context *ctx)
{
	t_token	*tok;
	t_ast	*node;

	tok = *ptokens;
	if (tok && tok->type == TOKEN_PIPE)
		return (set_parser_error(ctx, "pipe at start", tok), NULL);
	node = parser_command_or_subshell(&tok, ctx);
	if (!ctx->error_status && tok && tok->type == TOKEN_PIPE)
		node = parse_pipe_sequence(node, &tok, ctx);
	*ptokens = tok;
	return (node);
}

t_ast	*parser_subshell(t_token **ptokens, t_parser_context *ctx)
{
	t_token	*token;
	t_ast	*subshell;
	t_ast	*cmd;

	token = *ptokens;
	cmd = NULL;
	if (token && token->type == TOKEN_LPAREN)
	{
		token = token->next;
		subshell = parser_logical(&token, ctx);
		if (ctx->error_status)
			return (NULL);
		if (!token || token->type != TOKEN_RPAREN)
		{
			set_parser_error(ctx, "missing the ')'", token);
			ast_free(subshell);
			return (NULL);
		}
		token = token->next;
		cmd = ast_new_subshell(subshell);
	}
	*ptokens = token;
	return (cmd);
}

t_ast	*parser_command_or_subshell(t_token **ptokens, t_parser_context *ctx)
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
