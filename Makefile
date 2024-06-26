# Compiler
CC = cc

# Compiler flags
CFLAGS = -g -Wall -Wextra -Werror -I includes/ -I libft/includes/

#Libft
LIBFT_PATH	= libft/
LIBFT_NAME	= libft.a
LIBFT		= $(LIBFT_PATH)$(LIBFT_NAME)

# Source files
SRC_PATH = src/
SRC =	main.c \
		prompt.c \
		exec_utils.c \
		args_init.c \
		printer.c \
		mem_utils.c \
		parse_detection.c \
		builtin_main.c \
		base.c \
		redir.c \
		sig_handler.c \
		pipe.c \
		expansions.c \

SRCS		= $(addprefix $(SRC_PATH), $(SRC))

# Object files
OBJ_PATH	= obj/
OBJ			= $(SRC:.c=.o)
OBJS		= $(addprefix $(OBJ_PATH), $(OBJ))

# Target executable
NAME = minishell

# Default target
all: $(LIBFT) $(NAME)

# Other Prototypes
RM = rm -rf
# Compile source files into object files
$(OBJ_PATH)%.o: $(SRC_PATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS): $(OBJ_PATH)

$(OBJ_PATH):
	@mkdir $(OBJ_PATH)

$(LIBFT):
	make -C $(LIBFT_PATH)

# Link object files into target executable
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) -lreadline
# Clean up object files and target executable
clean:
	rm -rf $(OBJS) $(NAME)
	make clean -C $(LIBFT_PATH)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT_PATH)$(LIBFT_NAME)

re: fclean all

.PHONY: all re clean fclean