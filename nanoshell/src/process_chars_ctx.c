/*
 ▄▄▄██▀▀▀    ▄▄▄          ██▒   █▓    ██▓   ▓█████     ██▀███  
   ▒██      ▒████▄       ▓██░   █▒   ▓██▒   ▓█   ▀    ▓██ ▒ ██▒
   ░██      ▒██  ▀█▄      ▓██  █▒░   ▒██▒   ▒███      ▓██ ░▄█ ▒
▓██▄██▓     ░██▄▄▄▄██      ▒██ █░░   ░██░   ▒▓█  ▄    ▒██▀▀█▄  
 ▓███▒       ▓█   ▓██▒      ▒▀█░     ░██░   ░▒████▒   ░██▓ ▒██▒
 ▒▓▒▒░       ▒▒   ▓▒█░      ░ ▐░     ░▓     ░░ ▒░ ░   ░ ▒▓ ░▒▓░
 ▒ ░▒░        ▒   ▒▒ ░      ░ ░░      ▒ ░    ░ ░  ░     ░▒ ░ ▒░
 ░ ░ ░        ░   ▒           ░░      ▒ ░      ░        ░░   ░ 
 ░   ░            ░  ░         ░      ░        ░  ░      ░     

proces_char_ctx.c
                              ░                                
*/

#include "nanoshell.h"

int handle_single_quote(t_word_ctx *ctx, const char *line,
                               size_t *i, size_t len)
{
	if (!ctx || !line || !i)
		return (-1);
	ctx->seen_single = 1;
	(*i)++;
	if (parse_single_quote(&ctx->buf, line, i, len) < 0)
		return (-1);
	return (0);
}


int handle_double_quote(t_word_ctx *ctx, const char *line,
                               size_t *i, size_t len, int last_status)
{
	if (!ctx || !line || !i)
		return (-2);
	ctx->seen_double = 1;
	(*i)++;
	if (parse_double_quote(&ctx->buf, line, i, len, last_status) < 0)
		return (-2);
	return (0);
}

int handle_dollar(t_word_ctx *ctx, const char *line,
                         size_t *i, size_t len, int last_status)
{
	if (!ctx || !line || !i)
		return (-4);
	if (expand_dollar(&ctx->buf, line, i, len, last_status) < 0)
		return (-4);
	return (0);
}

int handle_regular_char(t_word_ctx *ctx, const char *line, size_t *i)
{
	if (!ctx || !line || !i)
		return (-3);
	ctx->seen_unquoted = 1;
	if (buf_append_char(&ctx->buf, line[*i]) < 0)
		return (-3);
	(*i)++;
	return (0);
}

int process_chars_ctx(t_word_ctx *ctx, const char *line,
                             size_t *i, size_t len, int last_status)
{
	int rc;

	while (*i < len && !is_space((char)line[*i]) &&
		   !is_operator((char)line[*i]))
	{
		rc = dispatch_char(ctx, line, i, len, last_status);
		if (rc != 0)
			return (rc);
	}
	return (0);
}
