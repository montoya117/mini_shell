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

static int  append_operator(const char *line, size_t *i, size_t len, t_token **head)
{
    t_token *op;

    op = parse_operator(line, i, len);
    if (!op)
    {
        free_tokens(*head);
        return (0);
    }
    token_append(head, op);
    return (1);
}

static int  append_word(const char *line, size_t *i, size_t len, t_token **head)
{
    t_token *word;

    word = parse_word(line, i, len);
    if (!word)
    {
        free_tokens(*head);
        return (0);
    }
    token_append(head, word);
    return (1);
}
t_token *tokenizer(const char *line)
{
    t_token *head;
    size_t  i;
    size_t  len;

	len = ft_strlen(line);
	head = NULL;
    i = 0;
	if (!line)
        return NULL;
    while (i < len)
	{
		skip_spaces(line, &i, len);
		if (i >= len)
			break ;
		if (line[i] == '<' || line[i] == '>' || line[i] == '|')
		{
			if (!append_operator(line, &i, len, &head))
                return (NULL);
            continue;
		}
		if (!append_word(line, &i, len, &head))
			return (NULL);
	}
	return (head);
}
