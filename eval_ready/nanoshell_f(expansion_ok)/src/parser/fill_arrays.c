/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_arrays.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alemonto <alemonto@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 15:56:20 by alemonto          #+#    #+#             */
/*   Updated: 2026/02/19 16:08:41 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static char	*str_join2(const char *a, const char *b)
{
	if (!a)
		return (ft_strdup(b ? b : ""));
	if (!b)
		return (ft_strdup(a));
	return (ft_strjoin(a, b));
}

static int	is_empty_str(const char *s)
{
	return (!s || s[0] == '\0');
}

static int	fields_push(char ***arr, int *len, char *s)
{
	char	**narr;
	int		i;

	narr = safe_malloc(sizeof(char *) * (*len + 2));
	i = 0;
	while (i < *len)
	{
		narr[i] = (*arr)[i];
		i++;
	}
	narr[*len] = s;
	narr[*len + 1] = NULL;
	free(*arr);
	*arr = narr;
	(*len)++;
	return (1);
}

/*
Regla clave:
- piezas quoted: concatenan SIEMPRE al campo actual (no splitting).
- piezas unquoted: se hace IFS splitting; el primer “word” pega al prefijo;
  los siguientes generan nuevos argv.
- leading/trailing IFS en una pieza unquoted pueden “cerrar” campo si ya tocado.
*/
static char	**pieces_to_fields(t_piece *p)
{
	char		**fields;
	int			nfields;
	char		*cur;
	int			touched;

	fields = NULL;
	nfields = 0;
	cur = ft_strdup("");
	touched = 0;
	while (p)
	{
		if (p->quoted)
		{
			char	*newcur = str_join2(cur, p->text ? p->text : "");

			free(cur);
			cur = newcur;
			touched = 1;
		}
		else
		{
			t_ifs_split	r;
			int			j;

			r = split_unquoted_ifs(p->text ? p->text : "");
			j = 0;
			if (r.leading_ifs && !is_empty_str(cur))
			{
				fields_push(&fields, &nfields, cur);
				cur = ft_strdup("");
				touched = 0;
			}
			while (r.words && r.words[j])
			{
				if (is_empty_str(cur))
				{
					free(cur);
					cur = ft_strdup(r.words[j]);
				}
				else
				{
					char	*newcur = str_join2(cur, r.words[j]);

					free(cur);
					cur = newcur;
				}
				touched = 1;
				if (r.words[j + 1])
				{
					fields_push(&fields, &nfields, cur);
					cur = ft_strdup("");
					touched = 0;
				}
				j++;
			}
			if (r.trailing_ifs && touched)
			{
				fields_push(&fields, &nfields, cur);
				cur = ft_strdup("");
				touched = 0;
			}
			free_ifs_split(r);
		}
		p = p->next;
	}
	if (touched)
		fields_push(&fields, &nfields, cur);
	else
		free(cur);
	return (fields);
}

static int	append_fields_to_argv(t_cmd_info *info, char **fields, int *i)
{
	int	j;

	j = 0;
	while (fields && fields[j])
	{
		info->argv[*i] = fields[j];
		info->argv_quoted[*i] = 0;
		(*i)++;
		j++;
	}
	free(fields);
	return (1);
}

static t_token	*fill_assignments(t_cmd_info *info, t_token *tok)
{
	int	i;

	info->assigns = safe_malloc((info->assign_count + 1) * sizeof(char *));
	i = 0;
	while (i < info->assign_count)
	{
		info->assigns[i++] = ft_strdup(tok->text);
		tok = tok->next;
	}
	info->assigns[i] = NULL;
	return (tok);
}

int	fill_arrays(t_cmd_info *info, t_token *tok)
{
	int		i;
	char	**fields;

	(void)tok;
	fill_assignments(info, tok);
	info->argv = safe_malloc((info->argc + 1) * sizeof(char *));
	info->argv_quoted = safe_malloc((info->argc + 1) * sizeof(int));
	i = 0;
	tok = info->words_start;
	while (tok && (tok->type == TOKEN_WORD || tok->type == TOKEN_EXPANSION
			|| is_redir_token(tok->type)))
	{
		if (is_redir_token(tok->type))
		{
			tok = tok->next;
			if (tok)
				tok = tok->next;
			continue ;
		}
		if (!tok->is_io_number)
		{
			if (tok->pieces)
				fields = pieces_to_fields(tok->pieces);
			else
				fields = (tok->text ? split_by_ifs(tok->text) : NULL);
			append_fields_to_argv(info, fields, &i);
		}
		tok = tok->next;
	}
	info->argv[i] = NULL;
	info->argv_quoted[i] = 0;
	return (1);
}