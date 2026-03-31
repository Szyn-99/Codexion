NAME = codexion
HEADER = codexion.h
FLAGS = -g -Wall -Wextra -Werror -pthread
SOURCE_FILE = codexion.c \
			parsing_arguments.c \
			codexion_free_utils.c \
			codexion_init.c \
			codexion_utils.c \
			priority_queue.c \
			codexion_croutine.c \
			codexion_monitor.c \
			codexion_release_catch.c

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