NAME			=	ft_ping

SRCS			=	main.c \
					options/options.c \
					utils/utils.c

OBJS			=	${SRCS:.c=.o}
CC				=	gcc
FLAGS			=	-Wall -Werror -Wextra -g
INCLUDE_DIRS	=	.
SANITIZER		=	-fsanitize=address

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) -I$(INCLUDE_DIRS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIRS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

sanitize:
	$(CC) $(FLAGS) $(SANITIZER) $(OBJS) -o $(NAME) -I$(INCLUDE_DIRS)

.PHONY: all clean fclean re



