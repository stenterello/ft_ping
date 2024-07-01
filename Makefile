NAME		=	ft_ping

SRCS		=	main.c \
			forge.c \
			options/options.c \
			utils/utils.c \
			strategies/default.c

OBJS		=	${SRCS:.c=.o}
CC		=	gcc
FLAGS		=	-Wall -Werror -Wextra -g
INCLUDE_DIRS	=	.

all: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) -I$(INCLUDE_DIRS)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@ -I$(INCLUDE_DIRS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re



