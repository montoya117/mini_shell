#include "nanoshell.h"

const char *display_text_for_token(const t_token *t)
{
	if (t == NULL)
		return "";

	if (t->text && t->text[0] != '\0')
		return t->text;

	/* token->text es NULL o vacío: mostrar símbolo según tipo */
	switch (t->type)
	{
		case TOKEN_PIPE: return "|";
		case TOKEN_REDIR_OUT: return ">";
		case TOKEN_REDIR_APPEND: return ">>";
		case TOKEN_REDIR_IN: return "<";
		case TOKEN_HEREDOC: return "<<";
		case TOKEN_EOF: return "EOF";
		case TOKEN_ERROR: return "ERROR";
		case TOKEN_WORD:
		default:
			/* fallback (debería haberse cubierto con text) */
			return "";
	}
}

/* Imprime tokens de una lista enlazada: token1 [texto] ... */
void tokens_print_simple(const t_token *head)
{
	const t_token *cur = head;
	int idx = 1;

	while (cur)
	{
		const char *txt = display_text_for_token(cur);
		printf("token%d [%s]\n", idx, txt);
		cur = cur->next;
		idx++;
	}
}

/* Variente para array de punteros: tokens_print_simple_array(arr, count) */
void tokens_print_simple_array(const t_token **arr, size_t count)
{
	size_t i;
	for (i = 0; i < count; ++i)
	{
		const t_token *t = arr[i];
		const char *txt = display_text_for_token(t);
		printf("token%zu [%s]\n", i + 1, txt);
	}
}