NAME = codexion
HEADER = codexion.h
FLAGS = -Wall -Wextra -Werror
SOURCE_FILE = codexion.c \
			parsing_arguments.c \
			parsing_arguments_tool.c \
			coders_routine.c \
			
OBJECT_FILE = $(SOURCE_FILE:.c=.o)

all: $(NAME)

$(NAME): $(OBJECT_FILE)
	cc $(FLAGS) $(OBJECT_FILE) -o $@

%.o: %.c $(HEADER)
	cc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECT_FILE)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all re fclean clean