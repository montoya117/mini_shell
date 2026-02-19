NAME    = minishell
HEADER  = includes/nanoshell.h
CC      = cc
CFLAGS  = -Wall -Werror -Wextra -g -Iincludes -ILIBFT2/includes# -fsanitize=address 
LDLIBS = -lreadline -lhistory -lncurses
OBJ_DIR = obj
SRC_DIR = src
LIBFT_DIR = LIBFT2
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS    = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: libft $(NAME)

libft:
	@echo "$(BLUE)Compiling libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDLIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Full clean completed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re libft

