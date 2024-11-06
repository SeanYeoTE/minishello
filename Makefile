# Compiler
CC = cc

# Compiler flags
CFLAGS = -g -Wall -Wextra -Werror

# Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Source files
SRC_PATH = src/
SRC =	main.c prompt.c checks.c checks2.c input_utils.c parse_detection.c scanner.c \
		exec_utils.c args_init.c base.c t_cmd_utils.c printer.c mem_utils.c \
		builtin_main.c cd.c echo.c env.c export.c export_utils.c pwd.c unset.c \
		exit.c var_handler.c var_utils.c redir.c sig_handler.c pipe.c single.c \
		expansions.c remove_quote.c heredoc.c

SRCS		= $(addprefix $(SRC_PATH), $(SRC))

# Object files
OBJ_PATH	= obj/
OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

# Header file
HEADER		= $(SRC_PATH)minishell.h

# Target executable
NAME = minishell

# Default target
all: $(LIBFT) $(NAME)

# Other Prototypes
RM = rm -rf

# Create object directory
$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

# Compile source files into object files
$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(HEADER) | $(OBJ_PATH)
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
