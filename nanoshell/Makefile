# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/08 13:07:06 by jadelgad          #+#    #+#              #
#    Updated: 2025/11/08 16:30:32 by jadelgad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
NAME = nanoshell

LIBFT_DIR = LIBFT2
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_INC = -I$(LIBFT_DIR)

#_______	sources		________#
SRC = nanoshell.c tokenizer.c basic_utils.c tokens_utils.c buffer_utils.c parse_operator.c
OBJ = $(SRC:.c=.o)



#_______	RULES		_______#

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME)
	@echo "$(NAME) COMPILED "

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

%.o: %.c nanoshell.h
	$(CC) $(CFLAGS) -I. $(LIBFT_INC) -c $< -o $@
	@echo "COMPILING...$<"

clean:
	@rm -f $(OBJ)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "OBJECTS CLEANED"

fclean:
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "ALL CLEANED"

re: fclean all

.PHONY: all clean fclean re

