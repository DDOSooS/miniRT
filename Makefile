CC = cc -g3 -fsanitize=address
CFLAGS = -Wall -Wextra -Werror # Additional flags can be added here
RM = rm -rf
LIBX = minilibx-linux/libmlx_Linux.a

LIBFT = ./includes/libft/libft.a
MLX = ./minilibx-linux/libmlx.a

name = miniRT
src = main.c\
	./src/parser/parsing.c \
	./src/parser/parse_shapes.c\
	./src/coloring/parser_texture.c\
	./src/coloring/coloring.c ./src/coloring/coloring_utils.c\
	./src/coloring/texturing.c\
	./src/coloring/checkering.c\
	./src/coloring/utils.c\
	./includes/get_next_line/get_next_line.c \
	./includes/get_next_line/get_next_line_utils.c \
	./src/operations/vec_utils.c ./src/operations/matrix.c ./src/operations/vec_utils1.c ./src/operations/vec_utils2.c ./src/operations/vec_utils3.c\
	./src/operations/matrix_utils.c ./src/operations/matrix_utils1.c ./src/operations/matrix_utils2.c\
	./src/operations/matrix_utils3.c ./src/operations/matrix_utils4.c ./src/operations/matrix_utils5.c ./src/operations/matrix_utils6.c \
	./src/operations/point.c ./src/operations/color_utils.c ./src/operations/color_utils1.c \
	./src/parser/checker_utils.c ./src/parser/ft_split.c ./src/parser/generate_map_utils.c ./src/parser/generate_map.c \
	./src/parser/parser_checker.c ./src/parser/parser_checker1.c ./src/parser/parser_checker2.c ./src/parser/parser_checker3.c \
	./src/parser/parser_checker4.c \
	./src/errors/error_handler.c\
	./src/components/ambient.c ./src/components/camera.c ./src/components/light.c ./src/components/plane.c ./src/components/spher.c\
	./src/components/cone_components.c ./src/components/cylinder.c\
	./src/raytracing/ray_utils.c \
	./src/freieer.c ./src/freieer_utils.c ./src/freieer_utils1.c\
	./src/loading.c\
	./src/raytracing/shapes/cone/cone.c\
	./src/raytracing/shapes/cone/cone_utils.c\
	./src/raytracing/shapes/cone/cone_trans.c\
	./src/raytracing/shapes/cone/cone_norm.c\
	./src/raytracing/shapes/cylinder/cylinder_norm.c\
	./src/raytracing/shapes/cylinder/cylinder_utils.c\
	./src/raytracing/shapes/cylinder/cylinder_rotation.c\
	./src/raytracing/shapes/cylinder/cylinder_intersection.c\
	./src/raytracing/shapes/cylinder/cylinder_intersection_utils.c\
	./src/raytracing/shapes/plan/plan_intersection.c\
	./src/raytracing/shapes/plan/plan_normal.c\
	./src/raytracing/shapes/plan/plan_utils.c\
	./src/raytracing/shapes/spher/spher_intersection.c\
	./src/raytracing/shapes/spher/spher_norm.c\
	./src/raytracing/shapes/spher/spher_utils.c\
	./src/raytracing/shapes/intersection_utils.c\
	./src/raytracing/shapes/pixel.c\
	./src/raytracing/shapes/shape.c\
	./src/raytracing/shapes/shape_utils.c\
	./src/raytracing/events/hooks.c\


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
.SECONDARY: $(obj)