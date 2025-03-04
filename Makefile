CC = cc -g3 #-fsanitize=address
CFLAGS = #-Wall # Additional flags can be added here
RM = rm -rf
LIBX = minilibx-linux/libmlx_Linux.a

LIBFT = ./includes/libft/libft.a
MLX = ./minilibx-linux/libmlx.a

name = scen	
src = main.c ./src/parser/parsing.c \
	./includes/get_next_line/get_next_line.c \
	./includes/get_next_line/get_next_line_utils.c \
	./src/operations/vec_utils.c ./src/raytracing/ray_utils.c

obj = $(src:.c=.o)

all: $(name)

$(name): $(obj) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) -o $@ $^ -Lminilibx-linux -lmlx_Linux -lX11 -lXext -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C ./includes/libft  # Proper rule to build libft.a

$(MLX):
	make -C ./minilibx-linux

clean:
	$(RM) $(obj)
	make -C ./includes/libft clean
	make -C ./minilibx-linux clean

fclean: clean
	$(RM) $(name)
	make -C ./includes/libft fclean
	# make -C ./minilibx-linux fclean

re: fclean all

.PHONY: re clean all fclean
