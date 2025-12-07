/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 13:31:36 by jadelgad          #+#    #+#             */
/*   Updated: 2025/11/13 13:32:43 by jadelgad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static int  append_word(const char *line, size_t *i, size_t len, t_token **head, int last_status, t_data *data)
{
    t_token *word;

    word = parse_word(line, i, len, last_status, data);
    if (!word)
    {
        free_tokens(*head);
        return (0);
    }
    token_append(head, word);
    return (1);
}

t_token *tokenizer(const char *line, int last_status, t_data *data)
{
    t_token *head;
    size_t  i;
    size_t  len;

    if (!line)
        return NULL;
    len = ft_strlen(line);
    head = NULL;
    i = 0;
    while (i < len)
    {
        skip_spaces(line, &i, len);
        if (i >= len)
            break ;
        if (line[i] == '<' || line[i] == '>' || line[i] == '|' ||
            line[i] == '&' || line[i] == '(' || line[i] == ')')
        {
            if (!append_operator(line, &i, len, &head))
                return (NULL);
            continue;
        }
        if (!append_word(line, &i, len, &head, last_status, data))
            return (NULL);
    }
    return (head);
}
