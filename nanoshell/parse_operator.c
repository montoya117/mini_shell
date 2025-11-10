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
                              ░                                
*/

#include "nanoshell.h"

static t_token *handle_greater(const char *line, size_t *i, size_t len, int pos)
{
    if (*i + 1 < len && line[*i + 1] == '>')
    {
        *i += 2;
        return (token_new(TOKEN_REDIR_APPEND, ft_strdup(">>"), QT_NONE, pos));
    }
    *i += 1;
    return (token_new(TOKEN_REDIR_OUT, ft_strdup(">"), QT_NONE, pos));
}
static t_token *handle_less(const char *line, size_t *i, size_t len, int pos)
{
    if (*i + 1 < len && line[*i + 1] == '<' )
    {
        *i += 2;
        return (token_new(TOKEN_HEREDOC, ft_strdup("<<"), QT_NONE, pos));
    }
    *i += 1
    return ((token_new(TOKEN_REDIR_IN, ft_strdup("<"), QT_NONE, pos));
}

static t_token *handle_pipe(const char *line, size_t *i, size_t len, int pos)
{
    (void)line;
    *i += 1;
    return (token_new(TOKEN_PIPE, ft_strdup("|"), QT_NONE, pos));
}

t_token *parse_operator(const char *line, size_t *i, size_t len)
{
    int pos;

    if (!line || !i)
        return (NULL);
    pos = (int)i;
    if (line[*i] == '>')
        return (handle_greater(line, i, len, pos));
    if (line[*i] == '<')
        return (handle_less(line, i, len, pos));
    if (line[*i] == '|')
        return (handle_pipe(line, i, len, pos));
    return (NULL);
}
