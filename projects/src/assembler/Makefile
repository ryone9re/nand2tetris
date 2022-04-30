NAME := hack_assembler
CC := gcc
RM := -rm
CFLAGS := -Wall -Wextra -Werror
C_LIST := main.c parse_tools.c symbol_table.c util.c
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
