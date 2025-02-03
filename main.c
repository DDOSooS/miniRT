#include "./includes/minirt.h"

t_map *ft_init_map()
{
    t_map *new;
    int i;

    i = -1;
    new = (t_map *)malloc(sizeof(t_map));
    if (!new)
        return NULL;
    new->lines = NULL;
    while (++i < 3)
        new->scen_elements[i] = 0;    
    return new;
}

int	ft_close(t_var *vars)
{
    printf("============> (hehooooooooooo) <============\n");
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0); // Exit the program
	return (0);
}

int	ft_close_window(t_var *data)
{
    mlx_destroy_window(data->mlx, data->win);

	exit(EXIT_SUCCESS);
	return (1);
}

inline int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void my_pixel_put(t_img *img, int x, int y, int color)
{
    int offset;

    offset = (img->line_length * y) + (x * (img->bits_per_pixel / 8));
    *((unsigned int *)(img->addr + offset)) = color;
}

void init_scene(t_scene *scene, float width, float height)
{
    scene->image_width = width;
    scene->aspect_ratio = 16.0f / 9.0f;
    // scene->image_height = (int)(scene->image_width / scene->aspect_ratio); // This will be about 225
    scene->image_height = height; 
    scene->vp_hight = 2.0;
    scene->vp_width = scene->vp_hight * scene->aspect_ratio;
    scene->camera->focal_lenght = 500; 
    
    scene->data = malloc(sizeof(t_var));
    scene->data->mlx = mlx_init();
    scene->data->win = mlx_new_window(scene->data->mlx, 
                                     scene->image_width, 
                                     scene->image_height, 
                                     "MiniRT");
    scene->data->img.img_ptr = mlx_new_image(scene->data->mlx, 
                                            scene->image_width, 
                                            scene->image_height);
    scene->data->img.addr = mlx_get_data_addr(scene->data->img.img_ptr,
                                             &scene->data->img.bits_per_pixel,
                                             &scene->data->img.line_length,
                                             &scene->data->img.endian);
}

int main(int argc, char **argv)
{
    t_map *map;
    t_scene *scene;

    if (argc != 2)
        return (printf("error: try with : ./minirt scene.rt\n"), 1);
    map = ft_init_map();
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_scen_map(argv[1]);
    if (!map->lines || !ft_check_map_components(&map))
        return (free(map), ft_putstr_fd("map is empty\n", 2), 1);
    scene = ft_generate_scene(map->lines);
    
    // var_dump_all(map, scene);
    float width = 700;
    float height = 400;
    init_scene(scene, width, height);
    // render_spheres(scene);
    t_world *world = default_world();
    
    // Create camera with 1:1 pixel mapping
    s_camera *camera = new_camera
    (
        height,
        width,
        180,    
        ft_new_point(-20, 100,-210), // Camera at origin
        ft_new_vector(0, 0, 1)  // Looking along z-axis
    );
    
    render_image(scene, world, camera);
    return 0;
}


