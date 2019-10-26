# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afonck <afonck@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 18:55:30 by afonck            #+#    #+#              #
#    Updated: 2019/10/26 15:06:09 by afonck           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libbmp.a

CC = clang

CFLAGS	= -Wall -Werror -Wextra

LDFLAGS = -L$(LIBFT_DIR)
LDLIBS = -lft

INCLUDES = -I$(LIBFT_DIR) -I$(INC_DIR)

LIBFT_DIR = ../libft/
LIBFT = $(LIBFT_DIR)libft.a
#LIBFT_HEADER = $(LIBFT_DIR)libft.h

SRC_DIR = ./src/
INC_DIR = ./includes/
OBJ_DIR = ./obj/
OBJ_LIST = $(SRC_LIST:.c=.o)

SRC_LIST = myBMPparser.c
HEADER_LIST = myBMPparser.h

BMP_SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))
BMP_OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))
BMP_HEADERS = $(addprefix $(INC_DIR), $(HEADER_LIST))

.PHONY: clean fclean all re

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ_DIR) $(BMP_OBJ)
	@ar rcs $(NAME) $(BMP_OBJ)

$(LIBFT):
	@echo "$(NAME): $(LIBFT) missing"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(NAME): $(OBJ_DIR) created"

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(BMP_HEADERS)
	@$(CC) $(CFLAGS) -c $(INCLUDES) $< $(LDFLAGS) $(LDLIBS) -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all
