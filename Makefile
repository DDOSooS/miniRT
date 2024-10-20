CC = cc
CFLAGS = -Wall # -fsanitize=address # Uncommenting options if needed 
RM = rm -rf
LIBFT = ./includes/libft/libft.a

name = miniRT
src = main.c ./parser/parsing.c \
	./includes/get_next_line/get_next_line.c \
	./includes/get_next_line/get_next_line_utils.c
obj = $(src:.c=.o)

all: $(name)

$(name): $(obj) $(LIBFT)
	$(CC) $(CFLAGS) $^ -o $@  # Linking with $(LIBFT) automatically since it's in $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C ./includes/libft  # Proper rule to build libft.a

clean:
	$(RM) $(obj)
	make -C ./includes/libft clean

fclean: clean
	$(RM) $(name)
	make -C ./includes/libft fclean

re: fclean all

.PHONY: re clean all fclean
