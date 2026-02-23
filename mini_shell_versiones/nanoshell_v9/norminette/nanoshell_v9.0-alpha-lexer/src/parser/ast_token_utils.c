/*
Token Utilities (in Parser)
------------------------------
t_token *token_next_word(t_token *tok);
Skips non-word tokens, returns next word token.

t_token *token_skip_until(t_token *tok, t_token_type type);
Skips tokens until it finds a desired type (e.g., pipe, redirect).
*/

#include "nanoshell.h"

t_token	*token_next_word(t_token *token)
{
	while (token && token->type != TOKEN_WORD)
		token = token->next;
	return (token);
}

t_token	*token_skip_until(t_token *token, t_token_type type)
{
	while (token && token->type != type)
		token = token->next;
	return (token);
}
