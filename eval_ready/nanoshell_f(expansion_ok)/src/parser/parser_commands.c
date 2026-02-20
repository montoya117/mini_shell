/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_commands.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jadelgad <jadelgad@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 15:22:05 by jadelgad          #+#    #+#             */
/*   Updated: 2026/02/19 16:15:48 by alemonto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nanoshell.h"

static int	handle_redirs(t_parser_context *ctx, t_ast **cmd,
						t_token **tok, t_cmd_info *i)
{
	int		type;
	char	*file;
	int		fd;

	while (*tok && is_redir_token((*tok)->type))
	{
		type = (*tok)->type;
		fd = check_io_number(type, i);
		*tok = (*tok)->next;
		if (!*tok || (*tok)->type != TOKEN_WORD)
			return (set_parser_error(ctx, "syntax error", *tok), 0);
		file = get_redir_file(type, (*tok)->text);
		if (!file)
			return (0);
		*cmd = ast_new_redirect(*cmd, file, type);
		if (fd == -1)
			fd = get_default_fd(type);
		(*cmd)->redirect_fd = fd;
		*tok = (*tok)->next;
		i->last_word = NULL;
	}
	return (1);
}

/* Cuenta argumentos y encuentra la 'last_word' */
static void	count_elements(t_cmd_info *info, t_token *tok)
{
	t_token	*tmp;

	tmp = tok;
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION)
		&& is_assignment_word(tmp->text))
	{
		info->assign_count++;
		tmp = tmp->next;
	}
	info->words_start = tmp;
	while (tmp && (tmp->type == TOKEN_WORD
			|| tmp->type == TOKEN_EXPANSION || is_redir_token(tmp->type)))
	{
		if (is_redir_token(tmp->type))
		{
			tmp = tmp->next;
			if (tmp)
				tmp = tmp->next;
			continue ;
		}
		info->argc += count_pieces(tmp->text, tmp->was_quoted);
		if (tmp->type == TOKEN_WORD)
			info->last_word = tmp;
		tmp = tmp->next;
	}
}

/* Reserva memoria y rellena argv/assignments 
static int	fill_arrays(t_cmd_info *info, t_token *tok)
{
	int		i;
	char	**pieces;
	int		j;

	info->assigns = safe_malloc((info->assign_count + 1) * sizeof(char *));
	info->argv = safe_malloc((info->argc + 1) * sizeof(char *));
	info->argv_quoted = safe_malloc((info->argc + 1) * sizeof(int));
	if (!info->assigns || !info->argv || !info->argv_quoted)
		return (0);
	i = 0;
	while (i < info->assign_count)
	{
		info->assigns[i++] = ft_strdup(tok->text);
		tok = tok->next;
	}
	info->assigns[i] = NULL;
	i = 0;
	tok = info->words_start;
	while (tok && (tok->type == TOKEN_WORD
			|| tok->type == TOKEN_EXPANSION || is_redir_token(tok->type)))
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
			if (tok->was_quoted)
			{
				info->argv[i] = ft_strdup(tok->text);
				info->argv_quoted[i] = 1;
				i++;
			}
			else
			{
				pieces = split_by_ifs(tok->text);
				j = 0;
				while (pieces && pieces[j])
				{
					info->argv[i] = ft_strdup(pieces[j]);
					info->argv_quoted[i] = 0;
					i++;
					j++;
				}
				free_split(pieces);
			}
		}
		tok = tok->next;
	}
	info->argv[i] = NULL;
	info->argv_quoted[i] = 0;
	return (1);
}
*/
/* Pega los arrays al comando base */
static void	attach_to_base(t_ast *cmd, t_cmd_info *i)
{
	t_ast	*base;

	base = cmd;
	while (base && base->type == AST_REDIRECT)
		base = base->left;
	if (base && base->type == AST_COMMAND)
	{
		base->argv = i->argv;
		base->assignments = i->assigns;
		base->argv_quoted = i->argv_quoted;
	}
}

static int	parse_mid_redirs(t_parser_context *ctx, t_ast **cmd,
	t_token **scan, t_cmd_info *info)
{
	while (*scan && ((*scan)->type == TOKEN_WORD
			|| (*scan)->type == TOKEN_EXPANSION
			|| is_redir_token((*scan)->type)))
	{
		if (is_redir_token((*scan)->type))
		{
			if (!handle_redirs(ctx, cmd, scan, info))
				return (0);
			continue ;
		}
		*scan = (*scan)->next;
	}
	return (1);
}

t_ast	*parser_commands(t_token **ptokens, t_parser_context *ctx)
{
	t_cmd_info	info;
	t_ast		*cmd;
	t_token		*tok;
	t_token		*scan;

	tok = *ptokens;
	ft_memset(&info, 0, sizeof(t_cmd_info));
	cmd = ast_new_command(NULL, NULL);
	if (!cmd || !handle_redirs(ctx, &cmd, &tok, &info))
		return (ast_free(cmd), NULL);
	count_elements(&info, tok);
	if (info.assign_count == 0 && info.argc == 0 && cmd->type != AST_REDIRECT)
		return (set_parser_error(ctx, "empty cmd", tok), ast_free(cmd), NULL);
	scan = info.words_start;
	if (!parse_mid_redirs(ctx, &cmd, &scan, &info))
		return (ast_free(cmd), NULL);
	if (!fill_arrays(&info, tok))
		return (ast_free(cmd), NULL);
	attach_to_base(cmd, &info);
	*ptokens = scan;
	return (cmd);
}
