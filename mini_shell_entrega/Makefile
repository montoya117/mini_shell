# ─── Colors ────────────────────────────────────────────────────────────────────
RED     = \033[0;31m
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
BLUE    = \033[0;34m
RESET   = \033[0m

# ─── Project ───────────────────────────────────────────────────────────────────
NAME      = minishell
HEADER    = includes/nanoshell.h
CC        = cc
CFLAGS    = -Wall -Werror -Wextra -g -Iincludes -ILIBFT2/includes
LDLIBS    = -lreadline
OBJ_DIR   = obj
LIBFT_DIR = LIBFT2
LIBFT_A   = $(LIBFT_DIR)/libft.a

# ─── Sources ───────────────────────────────────────────────────────────────────
SRCS    = src/main.c \
          src/builtins/builtin_cd.c \
          src/builtins/builtin_exit.c \
          src/builtins/builtin_exit_helpers.c \
          src/builtins/builtins.c \
          src/builtins/echo_pwd_env.c \
          src/builtins/export.c \
          src/builtins/helper_cd.c \
          src/builtins/unset.c \
          src/builtins/utils_helper_cd.c \
          src/env/assignments.c \
          src/env/env_utils.c \
          src/env/env_utils_helper.c \
          src/executor/exec_ast.c \
          src/executor/exec_command.c \
          src/executor/exec_command_helpers.c \
          src/executor/exec_pipeline.c \
          src/executor/exec_redir.c \
          src/executor/exec_redir_helper.c \
          src/executor/exec_subshell.c \
          src/executor/exec_utils.c \
          src/executor/find_path.c \
          src/executor/find_path_helper.c \
          src/executor/heredoc_expan.c \
          src/executor/heredoc_utils.c \
          src/executor/pipe_helpers.c \
          src/executor/pipe_utils.c \
          src/executor/run_multi_pipes.c \
          src/executor/run_pipeline.c \
          src/executor/spawn_child_helpers.c \
          src/gnl/get_next_line.c \
          src/lexer/buffer_utils.c \
          src/lexer/buffer_utils_helper.c \
          src/lexer/display_token.c \
          src/lexer/display_token_helper.c \
          src/lexer/display_token_utils.c \
          src/lexer/expand.c \
          src/lexer/expand_handlers.c \
          src/lexer/handle_brace_utils.c \
          src/lexer/process_chars_ctx.c \
          src/lexer/process_chars_ctx_handlers.c \
          src/lexer/tokenizer.c \
          src/lexer/tokenizer_helper.c \
          src/lexer/token_operator.c \
          src/lexer/token_quotes.c \
          src/lexer/token_utils.c \
          src/lexer/token_words.c \
          src/parser/ast_error.c \
          src/parser/ast_error_helper.c \
          src/parser/ast_init.c \
          src/parser/ast_init_helper.c \
          src/parser/ast_token_utils.c \
          src/parser/ast_utils.c \
          src/parser/fill_arrays.c \
          src/parser/parser_all.c \
          src/parser/parser_all_helpers.c \
          src/parser/parser.c \
          src/parser/parser_commands.c \
          src/parser/parser_commands_helpers.c \
          src/parser/parser_ifs.c \
          src/prompt/prompt_build.c \
          src/prompt/prompt_cwd.c \
          src/prompt/promt_user.c \
          src/prompt/promt_user_helper.c \
          src/signals/signals.c \
          src/utils/basic_utils.c \
          src/utils/basic_utils_helper.c \
          src/utils/main_helpers.c

OBJS    = $(SRCS:src/%.c=$(OBJ_DIR)/%.o)

# ─── Rules ─────────────────────────────────────────────────────────────────────
all: libft $(NAME)

libft:
	@echo "$(BLUE)Compilando libft...$(RESET)"
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT_A)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBFT_DIR) -lft $(LDLIBS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compilado correctamente!$(RESET)"

$(OBJ_DIR)/%.o: src/%.c $(HEADER)
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compilando $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "$(RED)Limpiando object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@echo "$(RED)Limpiando $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Full limpieza completa!$(RESET)"

re:
	@$(MAKE) fclean
	@$(MAKE) all

.PHONY: all clean fclean re libft
