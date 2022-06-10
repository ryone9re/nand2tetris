NAME := jack_compiler
CC := gcc
RM := -rm
CFLAGS := -Wall -Wextra -Werror
C_LIST := jack_analyzer.c jack_tokenizer.c symbol_table.c vm_writer.c compilation_engine_xml.c utils.c
H_LIST := include
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
