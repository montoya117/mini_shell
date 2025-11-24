

#include "nanoshell.h"

void	set_parser_error(t_parser_context *ctx, const char *message, t_token *token)
{
	if (ctx->error_status)
		return ;
	ctx->error_status = 1;
    ctx->error_message = message;
    ctx->error_token = token;
    ast_parser_error(ctx->error_message, token);
}

void	ctx_init(t_parser_context *ctx)
{
	ctx->error_status = 0;
	ctx->error_message = NULL;
	ctx->error_token = NULL;
}

t_ast   *parser(t_token **ptokens)
{
	t_parser_context	ctx;
	t_ast				*tree;

	ctx_init(&ctx);
    tree = parser_logical(ptokens,  &ctx);
	
	// check for parse errors
	if (ctx.error_status)
	{
		ast_parser_error(ctx.error_message, ctx.error_token);
		if (tree)
			ast_free(tree);
		return (NULL);
	}
	// check for leftover (unexpected) tokens
	if (*ptokens != NULL)
	{
		set_parser_error(&ctx, "unexpected extra input", *ptokens);
		ast_parser_error(ctx.error_message, ctx.error_token);
		if (tree)
            ast_free(tree);
        return (NULL);
	}
	return (tree);
}

