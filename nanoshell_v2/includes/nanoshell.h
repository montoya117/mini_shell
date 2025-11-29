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
	char			**argv; // for commands
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
t_token	*tokenizer(const char *line, int last_status);

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
t_token *parse_word(const char *line, size_t *i, size_t len, int last_status);

//________		TOKEN_QUOTES.C
int		parse_single_quote(t_buf *buf, const char *line, size_t *i, size_t len);
int		parse_double_quote(t_buf *buf, const char *line, size_t *i, size_t len, int last_status);

//_________		BUFFER_UTILS.C
int buf_append_str(t_buf *b, const char *s); // habra que moverla 
void    buf_init(t_buf *b);
void    buf_free(t_buf *b);
int		buf_ensure_capacity(t_buf *b, size_t min_needed);
int		buf_append_char(t_buf *b, char c);
char    *buf_release(t_buf *buf);//habra q moverla de este archivo, ya le buscare sitio...


//________        EXPAND.C
int expand_dollar(t_buf *buf, const char *line, size_t *i, size_t len, int last_status);
int expand_special_pid(t_buf *buf, size_t *i);
int expand_special_status(t_buf *buf, size_t *i, int last_status);

//__________    EXPAND_HANDLERS.C

int handle_braced(t_buf *buf, const char *line, size_t *i, size_t len);
int handle_simple(t_buf *buf, const char *line, size_t *i, size_t len);

//__________  HANDLE_BRACE_UTILS.C
size_t parse_identifier_len(const char *line, size_t i, size_t len);
char *ft_strndup(const char *s, size_t n);

//_______   PROCES_CHARS_CTX.C
int process_chars_ctx(t_word_ctx *ctx, const char *line,
                             size_t *i, size_t len, int last_status);
int handle_single_quote(t_word_ctx *ctx, const char *line, size_t *i, size_t len);
int handle_double_quote(t_word_ctx *ctx, const char *line,
                              size_t *i, size_t len, int last_status);
int handle_dollar(t_word_ctx *ctx, const char *line,
                              size_t *i, size_t len, int last_status);
int handle_regular_char(t_word_ctx *ctx, const char *line, size_t *i);

//______    PROCECES_CHARS_CTX_HANDLERS.C
int handle_backslash_outside(t_word_ctx *ctx, const char *line, size_t *i, size_t len);
int dispatch_char(t_word_ctx *ctx, const char *line, size_t *i, size_t len, int last_status);


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
t_ast	*ast_new_command(char **tokens);
t_ast	*ast_new_pipe(t_ast_list *children);
t_ast	*ast_new_redirect(t_ast *cmd, char *file, int redirect_type);
t_ast	*ast_new_and(t_ast *left, t_ast *right);
t_ast	*ast_new_or(t_ast *left, t_ast *right);
t_ast	*ast_new_subshell(t_ast *child);

//___________   AST_UTILS.C
t_ast_list *ast_list_new(t_ast *node);
t_ast_list *ast_list_append(t_ast_list *head, t_ast *node);
int			ast_count_args(char **argv);

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

//____________ EXEC_REDIR.C ______________
int	apply_redirect_and_exec(t_ast *node, t_data *data);

//___________EXEC_UTILS.C
void    exec_error(const char *message, const char *arg);
char *heredoc_tmp_name(void);
char *create_heredoc_tmp(char *delimeter);

//___________EXEC_COMMAND.C__________
int     exec_command(t_ast *node, t_data *data);

//__________FIND_PATH.C_______
char    *find_path(char *cmd, char **ev);


//_________FIND_PATH_HELPER.C
int	    calc_need_and_slash(const char *dir,
    const char *cmd,size_t *need, size_t *add_slash);



#endif

