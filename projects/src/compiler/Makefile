MAKEFILE_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
NAME := jack_compiler
CC := gcc
RM := -rm
CFLAGS := -Wall -Wextra -Werror
C_LIST := jack_analyzer.c jack_tokenizer.c symbol_table.c vm_writer.c compilation_engine.c utils.c
H_LIST := $(MAKEFILE_DIR)include
O_LIST := $(patsubst %.c, %.o, $(C_LIST))

$(NAME): $(O_LIST)
	$(CC) $(CFLAGS) -I $(H_LIST) $(O_LIST) -o $@

$(O_LIST):
	$(CC) $(CFLAGS) -I $(H_LIST) $(C_LIST) -c

all: $(NAME)

clean:
	$(RM) $(O_LIST)
.PHONY: clean

fclean: clean
	$(RM) $(NAME)
.PHONY: fclean

re: fclean all
