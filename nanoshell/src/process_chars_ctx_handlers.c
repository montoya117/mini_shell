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

PROCES_CHARS_CTS_HANDLERS.C
                              ░                                
*/

#include "nanoshell.h"

int dispatch_char(t_word_ctx *ctx, const char *line, size_t *i, size_t len, int last_status)
{
    char c = line[*i];

    if (c == '\'')
        return (handle_single_quote(ctx, line, i, len));
    if (c == '"')
        return (handle_double_quote(ctx, line, i, len, last_status));
    if (c == '$')
        return (handle_dollar(ctx, line, i, len, last_status));
    if (c == '\\')
        return (handle_backslash_outside(ctx, line, i, len));
    return (handle_regular_char(ctx, line, i));
}

int handle_backslash_outside(t_word_ctx *ctx, const char *line, size_t *i, size_t len)
{
    ctx->seen_unquoted = 1;
    (*i)++;
    if (*i < len)
    {
        if (buf_append_char(&ctx->buf, line[*i]) < 0)
            return (-3);
        (*i)++;
        return (0);
    }
    if (buf_append_char(&ctx->buf, '\\') < 0)
        return (-3);
    return (0);
}
