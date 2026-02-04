#include "nanoshell.h"

// Ayudante 1: Verifica si el último token palabra 
// era un FD (ej: el '2' en 2>)
static int	check_io_number(int type, t_cmd_info *i)
{
	if ((type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND)
		&& i->last_word
		&& i->last_word->type == TOKEN_WORD
		&& is_all_digits(i->last_word->text)
		&& i->last_word->join_next
		&& !i->last_word->was_quoted)
	{
		i->last_word->is_io_number = 1;
		return (ft_atoi(i->last_word->text));
	}
	return (-1);
}

static int	is_redir_token(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

static int	get_default_fd(int type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_HEREDOC)
		return (0);
	return (1);
}

/* Ayudante 2: Procesa redirecciones y crea nodos AST */
static char	*get_redir_file(int type, char *text)
{
	if (type == TOKEN_HEREDOC)
		return (create_heredoc_tmp(text));
	return (ft_strdup(text));
}

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

/* Ayudante 3: Cuenta argumentos y encuentra la 'last_word' */
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
	while (tmp && (tmp->type == TOKEN_WORD || tmp->type == TOKEN_EXPANSION))
	{
		if (tmp->type == TOKEN_WORD)
			info->last_word = tmp;
		info->argc++;
		tmp = tmp->next;
	}
}

/* Ayudante 4: Reserva memoria y rellena argv/assignments */
static int	fill_arrays(t_cmd_info *info, t_token *tok)
{
	int	i;

	info->assigns = safe_malloc((info->assign_count + 1) * sizeof(char *));
	info->argv = safe_malloc((info->argc + 1) * sizeof(char *));
	if (!info->assigns || !info->argv)
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
	while (tok && (tok->type == TOKEN_WORD || tok->type == TOKEN_EXPANSION))
	{
		if (!tok->is_io_number)
			info->argv[i++] = ft_strdup(tok->text);
		tok = tok->next;
	}
	info->argv[i] = NULL;
	return (1);
}

/* Ayudante 5: Pega los arrays al comando base */
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
	}
}

/* FUNCIÓN PRINCIPAL REFACTORIZADA */
t_ast	*parser_commands(t_token **ptokens, t_parser_context *ctx)
{
	t_cmd_info	info;
	t_ast		*cmd;
	t_token		*tok;
	t_token		*suffix_tok;

	tok = *ptokens;
	ft_memset(&info, 0, sizeof(t_cmd_info));
	cmd = ast_new_command(NULL, NULL);
	if (!cmd || !handle_redirs(ctx, &cmd, &tok, &info))
		return (ast_free(cmd), NULL);
	count_elements(&info, tok);
	if (info.assign_count == 0 && info.argc == 0 && cmd->type != AST_REDIRECT)
		return (set_parser_error(ctx, "empty cmd", tok), ast_free(cmd), NULL);
	suffix_tok = info.words_start;
	while (suffix_tok && (suffix_tok->type == TOKEN_WORD
			|| suffix_tok->type == TOKEN_EXPANSION))
		suffix_tok = suffix_tok->next;
	if (!handle_redirs(ctx, &cmd, &suffix_tok, &info))
		return (ast_free(cmd), NULL);
	if (!fill_arrays(&info, tok))
		return (ast_free(cmd), NULL);
	attach_to_base(cmd, &info);
	*ptokens = suffix_tok;
	return (cmd);
}
