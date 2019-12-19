# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afonck <afonck@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 18:55:30 by afonck            #+#    #+#              #
#    Updated: 2019/10/27 12:41:04 by afonck           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libbmp.a

CC = clang#gcc

CFLAGS	= -Wall -Werror -Wextra #-g

LDFLAGS = -L$(LIBFT_DIR) -L$(SDL2_DIR)lib
LDLIBS = -lft -lSDL2

INCLUDES = -I$(LIBFT_DIR) -I$(SDL2_DIR)include/SDL2/ -I$(INC_DIR)

CURRENT_DIR = $(shell pwd)

LIBFT_DIR = $(CURRENT_DIR)/../libft/
LIBFT = $(LIBFT_DIR)libft.a
SDL2_DIR = $(CURRENT_DIR)/../sdl2_lib/
LIBFT = $(LIBFT_DIR)libft.a
#LIBFT_HEADER = $(LIBFT_DIR)libft.h

SRC_DIR = ./src/
INC_DIR = ./includes/
OBJ_DIR = ./obj/
OBJ_LIST = $(SRC_LIST:.c=.o)

SRC_LIST = parse_bmp.c \
			parse_bmp_header.c \
			error_bmp.c \
			utils.c
HEADER_LIST = libbmp.h

BMP_SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))
BMP_OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))
BMP_HEADERS = $(addprefix $(INC_DIR), $(HEADER_LIST))

.PHONY: clean fclean all re

all: $(NAME)

$(NAME): $(LIBFT) $(SDL2) $(OBJ_DIR) $(BMP_OBJ)
	@ar rcs $(NAME) $(BMP_OBJ)

$(LIBFT):
	@echo "$(NAME): $(LIBFT) missing"

$(SDL2):
	@echo "$(NAME): $(SDL2) missing"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(NAME): $(OBJ_DIR) created"

$(OBJ_DIR)%.o : $(SRC_DIR)%.c $(BMP_HEADERS)
	@$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(OBJ_DIR) deleted"

fclean: clean
	@rm -f $(NAME)
	@echo "$(NAME) deleted"

re: fclean all
