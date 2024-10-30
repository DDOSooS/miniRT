CC = cc
CFLAGS = -Wall -I/home/ddos/Desktop/dev/42/miniRT/minilibx-linux  # Additional flags can be added here
RM = rm -rf
LIBMLX = -L/home/ddos/Desktop/dev/42/miniRT/minilibx-linux -lmlx -lX11 -lXext -lm  # Link MiniLibX with necessary libraries
LIBFT = ./includes/libft/libft.a
MLX = ./minilibx-linux/libmlx.a

name = miniRT
src = main.c ./parser/parsing.c \
	./includes/get_next_line/get_next_line.c \
	./includes/get_next_line/get_next_line_utils.c
obj = $(src:.c=.o)

all: $(name)

$(name): $(obj) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $^ $(LIBMLX) -o $@  

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C ./includes/libft  # Proper rule to build libft.a

$(MLX):
	make -C ./minilibx-linux

clean:
	$(RM) $(obj)
	make -C ./includes/libft clean
	# make -C ./minilibx-linux clean

fclean: clean
	$(RM) $(name)
	make -C ./includes/libft fclean
	# make -C ./minilibx-linux fclean

re: fclean all

.PHONY: re clean all fclean
