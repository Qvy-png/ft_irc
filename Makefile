# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rdel-agu <rdel-agu@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/23 17:29:46 by rdel-agu          #+#    #+#              #
#    Updated: 2023/01/23 18:02:03 by rdel-agu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98 -g3

RM = rm

OBJ = $(SRC:.cpp=.o)    

SRC =	srcs/main.cpp \
		srcs/ft_irc.cpp

INCL =	includes/color.hpp\
		includes/ft_irc.h\
		includes/ft_irc.hpp
		
all: $(NAME)

%.o: %.c 
	@$(CC) $(CFLAGS)  -I/usr/include -I INCL
		
$(NAME):  $(OBJ)
	@echo " \033[0;31mCompiling $(NAME)...\033[0m"
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	@echo " \033[0;32mSuccess\033[0m"
	
clean:
	@$(RM) -f $(OBJ) 
	@echo " \033[0;32mCleaning done!\033[0m"
	
fclean: clean
	@$(RM) -f $(NAME)

re: fclean all
	
.PHONY: clean fclean
