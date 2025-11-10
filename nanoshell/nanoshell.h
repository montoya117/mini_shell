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
# include "libft.h"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
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

/* struct mínimo usado por las funciones de impresión */
typedef struct s_token
{
	t_token_type	type;
	char			*text;
	t_quote_type	quote;
	int				pos;
	struct s_token	*next;
}	t_token;

typedef struct s_buf
{
    char    *data;
    size_t  len;
    size_t  cap;
}   t_buf;

//____________	BASIC_UTILS.C	____________

int		is_space(char c);
int		is_operator(char c);
void    skip_spaces(const char *line, size_t *i, size_t len);




//____________	TOKENS_UTILS.C

t_token *token_new(t_token_type type, char *text, t_quote_type qt, int pos);
void    token_append(t_token **head, t_token *node);
void    free_tokens(t_token *head);


//_________		BUFFER_UTILS.C

void    buff_init(t_buf *b);
void    buf_free(t_buf *b);
int		buff_ensure_capacity(t_buf *b, size_t min_needed);
int		buff_append_char(t_buf *b, char c);



//________      PARSE_OPERATOR.C
t_token *parse_operator(const char *line, size_t *i, size_t len);
#endif
