/*
      ▓█████  ███▄ ▄███▓ ▒█████    ██████  ██▓▓█████▄  ▒█████             
      ▓█   ▀ ▓██▒▀█▀ ██▒▒██▒  ██▒▒██    ▒ ▓██▒▒██▀ ██▌▒██▒  ██▒           
      ▒███   ▓██    ▓██░▒██░  ██▒░ ▓██▄   ▒██▒░██   █▌▒██░  ██▒           
      ▒▓█  ▄ ▒██    ▒██ ▒██   ██░  ▒   ██▒░██░░▓█▄   ▌▒██   ██░           
      ░▒████▒▒██▒   ░██▒░ ████▓▒░▒██████▒▒░██░░▒████▓ ░ ████▓▒░           
      ░░ ▒░ ░░ ▒░   ░  ░░ ▒░▒░▒░ ▒ ▒▓▒ ▒ ░░▓   ▒▒▓  ▒ ░ ▒░▒░▒░            
       ░ ░  ░░  ░      ░  ░ ▒ ▒░ ░ ░▒  ░ ░ ▒ ░ ░ ▒  ▒   ░ ▒ ▒░            
         ░   ░      ░   ░ ░ ░ ▒  ░  ░  ░   ▒ ░ ░ ░  ░ ░ ░ ░ ▒             
         ░  ░       ░       ░ ░        ░▀▀█▄    ░        ░ ░             
▓█████  ███▄    █   ▄████  ▄▄▄       ███▄    █ ░▄▄▄      ▓█████▄  ▒█████  
▓█   ▀  ██ ▀█   █  ██▒ ▀█▒▒████▄     ██ ▀█   █ ▒████▄    ▒██▀ ██▌▒██▒  ██▒
▒███   ▓██  ▀█ ██▒▒██░▄▄▄░▒██  ▀█▄  ▓██  ▀█ ██▒▒██  ▀█▄  ░██   █▌▒██░  ██▒
▒▓█  ▄ ▓██▒  ▐▌██▒░▓█  ██▓░██▄▄▄▄██ ▓██▒  ▐▌██▒░██▄▄▄▄██ ░▓█▄   ▌▒██   ██░
░▒████▒▒██░   ▓██░░▒▓███▀▒ ▓█   ▓██▒▒██░   ▓██░ ▓█   ▓██▒░▒████▓ ░ ████▓▒░
░░ ▒░ ░░ ▒░   ▒ ▒  ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ▒ ▒  ▒▒   ▓▒█░ ▒▒▓  ▒ ░ ▒░▒░▒░ 
 ░ ░  ░░ ░░   ░ ▒░  ░   ░   ▒   ▒▒ ░░ ░░   ░ ▒░  ▒   ▒▒ ░ ░ ▒  ▒   ░ ▒ ▒░ 
   ░      ░   ░ ░ ░ ░   ░   ░   ▒      ░   ░ ░   ░   ▒    ░ ░  ░ ░ ░ ░ ▒  
   ░  ░         ░       ░       ░  ░         ░       ░  ░   ░        ░ ░  
                                                          ░             */

#ifndef NANOSHELL_H
# define NANOSHELL_H

# include <stddef.h>
# include <unistd.h>
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <stdint.h>
# include "libft.h"
# include <errno.h>
# include <string.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

#define Y	"\033[1;33m"
#define G	"\033[1;32m"
#define C	"\033[1;36m"
#define M	"\033[1;35m"
#define RED	"\033[1;31m"
#define RST	"\033[0m"

#define DEL "\n\t \v\f\r"

extern volatile sig_atomic_t g_signal_received;

//________________ STRUCTS

// DATA STRUCT
typedef struct s_data {
	int		running;
	int		last_status;
	char	*cmd;
	char	*exit;
	char	**envp;
	int	(*pipes)[2];
	int	pipes_count;
}	t_data;

// TOKEN STRUCTS
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_EXPANSION,
	TOKEN_EOF,
	TOKEN_ERROR
}	t_token_type;

typedef enum e_quote_type
{
	QT_NONE,
	QT_SINGLE,
	QT_DOUBLE,
	QT_MIXED
}	t_quote_type;


typedef struct s_token
{
	t_token_type	type;
	char			*text;
	t_quote_type	quote;
	size_t			pos;
	struct s_token	*next;
}	t_token;

// BUFFER STRUCKS
typedef struct s_buf
{
    char    *data;
    size_t  len;
    size_t  cap;
}   t_buf;

typedef struct s_word_ctx
{
	t_buf	buf;
	int		seen_single;
	int		seen_double;
	int		seen_unquoted;
}	t_word_ctx;

// AST STRUCTS
typedef enum
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIRECT,
	AST_AND,
	AST_OR,
	AST_SUBSHELL
}	AST_TYPE;

typedef struct s_ast_list
{
	struct s_ast		*node;
	struct s_ast_list	*next;
}	t_ast_list;

typedef struct s_ast
{
	AST_TYPE		type;
	struct s_ast	*left; // for  && ||  and redirections
	struct s_ast	*right; // for && || and redirections
	char			**argv; // for command + normal args
	char			**assignments;  // NULL‑terminated array of "NAME=VALUE"
	char			*file; // for redirections
	int				redirect_type; // for rediretions
	t_ast_list		*children; // for pipe chain
}	t_ast;

// PARSER_STRUCTS
typedef struct s_parser_context
{
	int			error_status; // 0 if is OK, 1 if error is detected
	const char	*error_message; // message
	t_token		*error_token; // points to the error token
} t_parser_context;

// FUNCTIONS

                                // LEXER
//____________ TOKENIZER.CT
t_token	*tokenizer(const char *line, int last_status, t_data *data);

//____________	TOKENS_UTILS.C

t_token *token_new(t_token_type type, char *text, t_quote_type qt, int pos);
void    token_append(t_token **head, t_token *node);
void    free_tokens(t_token *head);
t_token *make_error_token_from_ctx(size_t start, const char *msg, t_word_ctx *ctx);

//____________	DISPLAY_TOKENS.C
const char	*display_text_for_token(const t_token *t);
void	tokens_print_simple(const t_token *head);
void	tokens_print_simple_array(const t_token **arr, size_t count);

//________      TOKEN_OPERATOR.C
t_token *parse_operator(const char *line, size_t *i, size_t len);

//________		TOKEN_WORDS.C
t_token *parse_word(const char *line, size_t *i, size_t len, int last_status, t_data *data);

//________		TOKEN_QUOTES.C
int		parse_single_quote(t_buf *buf, const char *line, size_t *i, size_t len);
int		parse_double_quote(t_buf *buf, const char *line, size_t *i, size_t len, int last_status, t_data *data);

//_________		BUFFER_UTILS.C
int		buf_append_str(t_buf *b, const char *s); // habra que moverla 
void    buf_init(t_buf *b);
void    buf_free(t_buf *b);
int		buf_ensure_capacity(t_buf *b, size_t min_needed);
int		buf_append_char(t_buf *b, char c);
char    *buf_release(t_buf *buf);//habra q moverla de este archivo, ya le buscare sitio...


//________        EXPAND.C
int expand_dollar(t_buf *buf, const char *line, size_t *i, size_t len, int last_status, t_data *data);
int expand_special_pid(t_buf *buf, size_t *i);
int expand_special_status(t_buf *buf, size_t *i, int last_status);

//__________    EXPAND_HANDLERS.C

int handle_braced(t_buf *buf, const char *line, size_t *i, size_t len, t_data *data);
int handle_simple(t_buf *buf, const char *line, size_t *i, size_t len, t_data *data);

//__________  HANDLE_BRACE_UTILS.C
size_t parse_identifier_len(const char *line, size_t i, size_t len);
char *ft_strndup(const char *s, size_t n);

//_______   PROCES_CHARS_CTX.C
int process_chars_ctx(t_word_ctx *ctx, const char *line,
                             size_t *i, size_t len, int last_status, t_data *data);
int handle_single_quote(t_word_ctx *ctx, const char *line, size_t *i, size_t len);
int handle_double_quote(t_word_ctx *ctx, const char *line,
                              size_t *i, size_t len, int last_status, t_data *data);
int handle_dollar(t_word_ctx *ctx, const char *line,
                              size_t *i, size_t len, int last_status, t_data *data);
int handle_regular_char(t_word_ctx *ctx, const char *line, size_t *i);

//______    PROCECES_CHARS_CTX_HANDLERS.C
int handle_backslash_outside(t_word_ctx *ctx, const char *line, size_t *i, size_t len);
int dispatch_char(t_word_ctx *ctx, const char *line, size_t *i, size_t len, int last_status, t_data *data);


//_________		SIGNALS.C
void	setup_signals(void);

                       //__________    PARSER

//__________    PARSER.C
t_ast   *parser(t_token **ptokens);
void    set_parser_error(t_parser_context *ctx, const char *message, t_token *token);
void	ctx_init(t_parser_context *ctx);
//__________    PARSER_COMMANDS.C
t_ast	*parser_commands(t_token **ptokens, t_parser_context *ctx);
t_ast	*parser_logical(t_token **ptokens, t_parser_context *ctx);
t_ast   *parser_pipe(t_token **ptokens, t_parser_context *ctx);
t_ast   *parser_subshell(t_token **ptokens, t_parser_context *ctx);
t_ast   *parser_command_or_subshell(t_token **ptokens, t_parser_context *ctx);

//__________    AST_TREE
//__________    AST_INIT.C
t_ast	*ast_new_command(char **tokens, char **assignments);
t_ast	*ast_new_pipe(t_ast_list *children);
t_ast	*ast_new_redirect(t_ast *cmd, char *file, int redirect_type);
t_ast	*ast_new_and(t_ast *left, t_ast *right);
t_ast	*ast_new_or(t_ast *left, t_ast *right);
t_ast	*ast_new_subshell(t_ast *child);

//___________   AST_UTILS.C
t_ast_list *ast_list_new(t_ast *node);
t_ast_list *ast_list_append(t_ast_list *head, t_ast *node);
int			ast_count_args(char **argv);
int		is_assignment_word(const char *s);

//___________   AST_PRINTING.C
const char *ast_to_string(const AST_TYPE type);
void 		ast_print(const t_ast *root, int indent);

//___________   AST_ERROR.C
void		ast_free(t_ast *node);
void		ast_list_free(t_ast_list *list);
void		ast_parser_error(const char *message, t_token *token);
// TO MOVE
int			ft_strlen_const(const char *str); // TODO HEY HEY !!

//___________   AST_TOKEN_UTILS.C
t_token		*token_next_word(t_token *token);
t_token		*token_skip_until(t_token *token, t_token_type type);
// ___________  UTILS
//____________	BASIC_UTILS.C	____________
int		is_space(char c);
int		is_operator(char c);
void    skip_spaces(const char *line, size_t *i, size_t len);
void	*safe_malloc(size_t size);

//___________________________________ EXECUTOR

//____________ EXEC_AST.C ______________
int     exec_ast(t_ast *node, t_data *data);
int		exec_ast_in_child(t_ast *node, t_data *data);

//____________ EXEC_REDIR.C ______________
int		apply_redirect_and_exec(t_ast *node, t_data *data);
void	apply_redirect_and_exec_child(t_ast *node, t_data *data);

//___________EXEC_COMMAND.C__________
int 	wait_for_child(pid_t pid);
void	exec_child(t_ast *node, t_data *data);
void	reset_sig_in_child(void);
int     exec_command(t_ast *node, t_data *data);

//___________EXEC_SUBSHELL.C__________
int exec_subshell(t_ast *node, t_data *data);

//___________EXEC_UTILS.C
void    exec_error(const char *message, const char *arg);
char *heredoc_tmp_name(void);
char *create_heredoc_tmp(char *delimeter);

//__________FIND_PATH.C_______
char    *find_path(char *cmd, char **envp);
int  try_path_segment(const char *seg, const char *cmd, char ** out);

//_________FIND_PATH_HELPER.C
int	    calc_need_and_slash(const char *dir,
    const char *cmd,size_t *need, size_t *add_slash);
int try_path_range(const char *path, size_t start, size_t i,
                          const char *cmd, char **res);
                          
//_________EXEC_PIPELINE.C
int exec_pipeline(t_ast *node, t_data *data);

//______RUN_PIPELINE.C
int run_single(t_ast *cmd, t_data *data);
pid_t spawn_child(t_ast *cmd, int in_fd, int out_fd, t_data *data);

//_____RUN_MULTI_PIPES.C
int run_multi(t_ast **cmds, int n, t_data *data);
int  spawn_all_children(t_ast **cmds, int n, int (*pipes)[2], t_data *data, pid_t *pids);
int allocate_resources(int n, int (*(*pipes_out))[2], pid_t **pids_out);
int  create_pipes(int (*pipes)[2], int n);

//_____PIPE_HELPERS.C
/*int forward_fd(int fd);*/
int wait_for_all(pid_t *pids, int count);
void close_all_pipes(int (*pipes)[2], int n);

//_____PIPE_UTILS.C
int spawn_children_checked(t_ast **cmds, int n, int (*pipes)[2],
				  t_data *data, pid_t *pids);
pid_t wait_one(pid_t pid, int *st);
int status_to_code(int s);
int setup_resources(int n, int (*(*pipes_out))[2], pid_t **pids_out, t_data *data);
void teardown_resources(int (*pipes)[2], pid_t *pids, int n, t_data *data);				  
				  
int status_to_code(int s);
pid_t wait_one(pid_t pid, int *st);

//_____ASSIGNMENTS.C
void		apply_assignment(char ***penv, const char *assignment);
void		apply_assignments_array(char ***penv, char **assignments);
int			is_assignment_word(const char *s);
int			find_name(char **envp, const char *name);
char		*join_name_value(const char *name, const char *value);

//_____ENV_UTILS.C
char    	**dup_env(char **envp);
int			set_env_var_generic(char ***penv, const char *name, const char *value);
int			set_env_var_data(t_data *data, const char *name, const char *value);
const char	*get_var_from_envp(char **envp, const char *name);
void		free_env(char **envp);


//_______BUILTINS.C
int 	call_builtin(char **argv, t_data *data);
int 	builtin_should_run_in_parent(const char *name);
int 	is_builtin(const char *name);
void 	builtin_error(const char *cmd, const char *arg, const char *message);

//______ECHO_PWD_ENV.C
int 	builtin_echo(char **argv, t_data *data);
int 	builtin_pwd(char **argv, t_data *data);
int 	builtin_env(char **argv, t_data *data);

//_________BUILTIN_CD.C
int 	builtin_cd(char **argv, t_data *data);

//________HELPER_CD.C
char 	*get_raw_cd_target(char **argv, t_data *data);

//_______UTILS_HELPER_CD.C
char 	*copy_home_and_rest(const char *home, const char *path);
char 	*expand_home(const char *path, t_data *data);
char 	*resolve_cd_target(char **argv, t_data *data);

//______EXPORT.C
int 	is_valid_identifier(char *arg);
void 	split_name_value(const char *arg, char **name, char **value);
int 	builtin_export(char **argv, t_data *data);
#endif

