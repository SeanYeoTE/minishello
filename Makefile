# Compiler
CC = gcc

# Compiler flags
CFLAGS = -g -Wall -Wextra -Werror -Iincludes

# Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Source directories
SRC_PATH = src/
CORE_DIR = 1.Core/
LEXER_DIR = 2.Lexer/
PARSER_DIR = 3.Parser/
EXEC_DIR = execution/
BUILTIN_DIR = builtins/
UTILS_DIR = utils/
VAR_DIR = variables/
REDIR_DIR = redirections/

# Source files by directory
CORE_SRC =		main.c sig_handler.c base.c setup_utils.c setup_getters.c exit_wrapper.c
LEXER_SRC =		scanner.c scanner_quote.c lexer.c checks.c checks2.c \
				input_quote.c input_space.c t_node_utils.c
PARSER_SRC = 	parser.c t_cmd_utils.c t_cmd_utils_extra.c
EXEC_SRC =		pipe_exec.c pipe_fd.c pipe_process.c pipe_setup.c \
				single.c exec_argv.c exec_fd.c exec_main.c exec_path.c \
				single_external.c
REDIR_SRC =		redir_handler.c redir_utils.c redir_io.c \
				heredoc_core.c heredoc_utils.c heredoc_wrapper.c
BUILTIN_SRC =	builtin_main.c cd.c echo.c env.c export.c export_utils.c pwd.c unset.c exit.c
UTILS_SRC =		printer.c printer_error.c mem_utils.c mem_utils_extra.c
VAR_SRC =		var_handler.c var_utils.c expansions.c expansions_utils.c expansions_replace.c \
				expansions_state.c remove_quote.c

# Combine all sources with their directories
SRCS =	$(addprefix $(SRC_PATH)$(CORE_DIR), $(CORE_SRC)) \
		$(addprefix $(SRC_PATH)$(LEXER_DIR), $(LEXER_SRC)) \
		$(addprefix $(SRC_PATH)$(PARSER_DIR), $(PARSER_SRC)) \
		$(addprefix $(SRC_PATH)$(EXEC_DIR), $(EXEC_SRC)) \
		$(addprefix $(SRC_PATH)$(REDIR_DIR), $(REDIR_SRC)) \
		$(addprefix $(SRC_PATH)$(BUILTIN_DIR), $(BUILTIN_SRC)) \
		$(addprefix $(SRC_PATH)$(UTILS_DIR), $(UTILS_SRC)) \
		$(addprefix $(SRC_PATH)$(VAR_DIR), $(VAR_SRC))

# Object files
OBJ_PATH = obj/
OBJ_DIRS = $(CORE_DIR) $(LEXER_DIR) $(PARSER_DIR) $(EXEC_DIR) $(BUILTIN_DIR) $(UTILS_DIR) \
			$(VAR_DIR) $(REDIR_DIR)
OBJS = $(SRCS:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

# Header files
HEADERS = includes/minishell.h includes/core.h includes/parser.h includes/builtins.h \
		  includes/execution.h includes/lexer.h includes/redirections.h includes/utils.h \
		  includes/variables.h

# Target executable
NAME = minishell

# Default target
all: $(LIBFT) $(NAME)

# Other Prototypes
RM = rm -rf

# Create object directories
$(OBJ_PATH)%/:
	@mkdir -p $@

# Compile source files into object files
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build libft
$(LIBFT):
	@make -C $(LIBFT_PATH)

# Link object files into target executable
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline 

# Clean up object files
clean:
	$(RM) $(OBJ_PATH)
	@make clean -C $(LIBFT_PATH)

# Clean up object files and target executable
fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re
