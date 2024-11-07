# Compiler
CC = cc

# Compiler flags
CFLAGS = -g -Wall -Wextra -Werror

# Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Source directories
SRC_PATH = src/
CORE_DIR = core/
PARSER_DIR = parser/
EXEC_DIR = execution/
BUILTIN_DIR = builtins/
UTILS_DIR = utils/
VAR_DIR = variables/

# Source files by directory
CORE_SRC = main.c sig_handler.c base.c setup_utils.c
PARSER_SRC = scanner.c parse_detection.c checks.c checks2.c input_utils.c
EXEC_SRC = exec_utils.c pipe.c single.c redir.c heredoc.c
BUILTIN_SRC = builtin_main.c cd.c echo.c env.c export.c export_utils.c pwd.c unset.c exit.c
UTILS_SRC = args_init.c t_cmd_utils.c printer.c mem_utils.c
VAR_SRC = var_handler.c var_utils.c expansions.c remove_quote.c

# Combine all sources with their directories
SRCS =	$(addprefix $(SRC_PATH)$(CORE_DIR), $(CORE_SRC)) \
		$(addprefix $(SRC_PATH)$(PARSER_DIR), $(PARSER_SRC)) \
		$(addprefix $(SRC_PATH)$(EXEC_DIR), $(EXEC_SRC)) \
		$(addprefix $(SRC_PATH)$(BUILTIN_DIR), $(BUILTIN_SRC)) \
		$(addprefix $(SRC_PATH)$(UTILS_DIR), $(UTILS_SRC)) \
		$(addprefix $(SRC_PATH)$(VAR_DIR), $(VAR_SRC))

# Object files
OBJ_PATH = obj/
OBJ_DIRS = $(CORE_DIR) $(PARSER_DIR) $(EXEC_DIR) $(BUILTIN_DIR) $(UTILS_DIR) $(VAR_DIR)
OBJS = $(SRCS:$(SRC_PATH)%.c=$(OBJ_PATH)%.o)

# Header file
HEADER = $(SRC_PATH)$(CORE_DIR)minishell.h

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
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADER)
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
