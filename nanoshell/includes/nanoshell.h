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
	int		status;
	char	*cmd;
	char	*exit;
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



// FUNCTIONS

// LEXER
//____________ TOKENIZER.CT
t_token	*tokenizer(const char *line);

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
t_token *parse_word(const char *line, size_t *i, size_t len);

//________		TOKEN_QUOTES.C
int		parse_single_quote(t_buf *buf, const char *line, size_t *i, size_t len);
int		parse_double_quote(t_buf *buf, const char *line, size_t *i, size_t len);

//_________		BUFFER_UTILS.C
void    buf_init(t_buf *b);
void    buf_free(t_buf *b);
int		buf_ensure_capacity(t_buf *b, size_t min_needed);
int		buf_append_char(t_buf *b, char c);
char    *buf_release(t_buf *buf);//habra q moverla de este archivo, ya le buscare sitio...

//_________		SIGNALS.C
void	setup_signals(void);

//__________    PARSER

//__________    PARSER.C
t_ast   *parser(t_token *tokens);

//__________    PARSER_COMMANDS.C
t_ast *parser_commands(t_token *tokens);

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


#endif

