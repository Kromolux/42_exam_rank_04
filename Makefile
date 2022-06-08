# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rkaufman <rkaufman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/08 18:24:01 by rkaufman          #+#    #+#              #
#    Updated: 2022/06/08 20:45:45 by rkaufman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		:=	microshell

CC			:=	gcc

SRCS		:=	main.c

OBJS		:=	$(SRCS:%.c=%.o)

CFLAGS		=	-Wall -Wextra -Werror
DFLAG		:=	-g -D DEBUG=1

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

debug: CFLAGS += $(DFLAG)
debug: re
	./$(NAME)

clean:
	rm -fr $(OBJS)

fclean: clean
	rm -fr $(NAME)

re: fclean all

val:
	valgrind --track-fds=yes ./$(NAME) /bin/ls "|" /usr/bin/grep microshell "|" /usr/bin/wc -l ";" /bin/echo i love my microshell ";" cd .. ";" /bin/ls ";" /bin/pwd ";" ";" ";" /bin/echo ABC "|" /usr/bin/wc -c

leak:
	leaks --atExit -- ./$(NAME) /bin/ls "|" /usr/bin/grep microshell "|" /usr/bin/wc -l ";" /bin/echo i love my microshell ";" cd .. ";" /bin/ls ";" /bin/pwd ";" ";" ";" /bin/echo ABC "|" /usr/bin/wc -c

.PHONY: clean fclean re
