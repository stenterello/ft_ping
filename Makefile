NAME			=	ft_ping

SRC				=	main.c \
					options.c \
					craft.c \
					utils.c \
					run.c \
					resolve_address.c \
					connection.c \
					calculations.c

SRCS			=	${addprefix src/, $(SRC)}
OBJS			=	${SRCS:.c=.o}
CC				=	gcc
FLAGS			=	-Wall -Werror -Wextra -g
INCLUDE_DIRS	=	./include
SANITIZER		=	-fsanitize=address
LIBS			=	-lm

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) -I$(INCLUDE_DIRS) $(LIBS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIRS) $(LIBS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

sanitize:
	$(CC) $(FLAGS) $(SANITIZER) $(OBJS) -o $(NAME) -I$(INCLUDE_DIRS)

.PHONY: all clean fclean re



