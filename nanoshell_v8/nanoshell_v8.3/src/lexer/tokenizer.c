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

static int  append_operator(const char *line, size_t *i, size_t len, t_token **head, int had_space)
{
    t_token *op;
    t_token *prev;

    op = parse_operator(line, i, len);
    if (!op)
    {
        free_tokens(*head);
        return (0);
    }
    prev = *head;
    while (prev && prev->next)
        prev = prev->next;
    if (prev)
    {
        if (had_space)
            prev->join_next = 0;
        else
            prev->join_next = 1;
    }
    token_append(head, op);
    return (1);
}

static int  append_word(const char *line, size_t *i, size_t len, t_token **head, int last_status, t_data *data, int had_space)
{
    t_token *word;
    t_token *prev;

    word = parse_word(line, i, len, last_status, data);
    if (!word)
    {
        free_tokens(*head);
        return (0);
    }
    prev = *head;
    while (prev && prev->next)
        prev = prev->next;
    if (prev)
    {
        if (had_space)
            prev->join_next = 0;
        else
            prev->join_next = 1;
    }
    token_append(head, word);
    return (1);
}

static int  skip_spaces_with_flag(const char *line, size_t *i, size_t len)
{
    size_t  before;

    before = *i;
    skip_spaces(line, i, len);
    if (*i > before)
        return (1);
    return (0);
}

t_token *tokenizer(const char *line, int last_status, t_data *data)
{
    t_token *head;
    size_t  i;
    size_t  len;
    int     had_space;

    if (!line)
        return NULL;
    len = ft_strlen(line);
    head = NULL;
    i = 0;
    while (i < len)
    {
        had_space = skip_spaces_with_flag(line, &i, len);
        if (i >= len)
            break ;
        if (line[i] == '<' || line[i] == '>' || line[i] == '|' ||
            line[i] == '&' || line[i] == '(' || line[i] == ')')
        {
            if (!append_operator(line, &i, len, &head, had_space))
                return (NULL);
            continue;
        }
        if (!append_word(line, &i, len, &head, last_status, data, had_space))
            return (NULL);
    }
    return (head);
}
