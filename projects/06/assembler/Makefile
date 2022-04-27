NAME := hack_assembler
CC := gcc
RM := -rm
CFLAGS := -Wextra -Wall -Werror
C_LIST := main.c parse_tools.c util.c
H_LIST := assembler.h
O_LIST := $(patsubst %.c, %.o, $(C_LIST))

$(NAME): $(O_LIST)
	$(CC) $(CFLAGS) -I $(H_LIST) $(O_LIST) -o $@

all: $(NAME)

clean:
	$(RM) $(O_LIST)
.PHONY: clean

fclean: clean
	$(RM) $(NAME)
.PHONY: fclean

re: fclean all
