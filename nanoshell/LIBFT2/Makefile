# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jadelgad <jadelgad@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/05 11:11:49 by jadelgad          #+#    #+#              #
#    Updated: 2025/05/20 13:38:05 by jadelgad         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Werror -Wextra

NAME = libft.a

SRC = \
	ft_strlen.c ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
	ft_isprint.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strchr.c ft_strrchr.c ft_strncmp.c ft_memchr.c ft_tolower.c \
	ft_toupper.c ft_strlcpy.c ft_strlcat.c ft_memcmp.c ft_strnstr.c \
	ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
	ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c
	
OBJ = $(SRC:.c=.o)

BONUS_SRC = \
    	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
    	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c \
    	ft_lstiter_bonus.c ft_lstmap_bonus.c

BONUS_OBJ = $(BONUS_SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)
	@echo "                 $(NAME)📚  is now created 🚀 "

bonus: $(OBJ) $(BONUS_OBJ)
	@ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)
	@echo "🧪  🧩  🧪  🧩  🧪 BONUS FUNCTIONS ADDED TO $(NAME)🧪  🧩  🧪  🧩  🧪" 

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "          🔧  I'm compiling as fast as I can $<"

clean:
	@rm -f $(OBJ) $(BONUS_OBJ)
	@echo "           🧹 🧹 🧹 🧹 🧹 🧹 🧹  Cleaned object files  🧹 🧹 🧹 🧹 🧹 🧹 🧹"

fclean: clean
	@rm -f $(NAME)
	@echo "           🗑️  🗑️  🗑️  🗑️  🗑️  🗑️   Trash removed   🗑️  🗑️  🗑️  🗑️  🗑️  🗑️  "

re: fclean all

.PHONY: all clean fclean re bonus
