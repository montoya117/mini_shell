/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_chars_ctx_handlers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 11:52:05 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/29 11:52:15 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

/* Dispatch con trazas

int dispatch_char(t_word_ctx *ctx, const char *line, size_t *i, size_t len, int last_status)
{
    char c = line[*i];
    fprintf(stderr, "[DBG] dispatch at i=%zu char='%c' (0x%02x)\n", *i, c, (unsigned char)c);
    dbg_buf_print(ctx);

    if (c == '\'')
    {
        int rc = handle_single_quote(ctx, line, i, len);
        fprintf(stderr, "[DBG] after handle_single_quote -> rc=%d i=%zu\n", rc, *i);
        dbg_buf_print(ctx);
        return rc;
    }
    if (c == '"')
    {
        int rc = handle_double_quote(ctx, line, i, len, last_status);
        fprintf(stderr, "[DBG] after handle_double_quote -> rc=%d i=%zu\n", rc, *i);
        dbg_buf_print(ctx);
        return rc;
    }
    if (c == '$')
    {
        int rc = handle_dollar(ctx, line, i, len, last_status);
        fprintf(stderr, "[DBG] after handle_dollar -> rc=%d i=%zu\n", rc, *i);
        dbg_buf_print(ctx);
        return rc;
    }
    if (c == '\\')
    {
        int rc = handle_backslash_outside(ctx, line, i, len);
        fprintf(stderr, "[DBG] after handle_backslash_outside -> rc=%d i=%zu\n", rc, *i);
        dbg_buf_print(ctx);
        return rc;
    }
    {
        int rc = handle_regular_char(ctx, line, i);
        fprintf(stderr, "[DBG] after handle_regular_char -> rc=%d i=%zu appended='%c'\n",
                rc, *i, (unsigned char)line[(*i>0)?(*i-1):0]);
        dbg_buf_print(ctx);
        return rc;
    }
}
*/

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
