NAME := hack-vm
CC := gcc
RM := -rm
CFLAGS := -Wall -Wextra -Werror
C_LIST := main.c
H_LIST := hack_vm.h
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
