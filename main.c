#include "./includes/minirt.h"

void var_dump_all(t_map *map, t_scene *scene)
{
    // ---  t_scene ---
    if (scene)
    {
        //  camera
        printf("\n--- Camera  %p---\n" , scene->camera);
        if (scene->camera)
        {
            printf("Camera Position: [%f, %f, %f]\n", scene->camera->origin.x, scene->camera->origin.y, scene->camera->origin.z);
            printf("Camera Coordinates: [%f, %f, %f]\n", scene->camera->direction.x, scene->camera->direction.y, scene->camera->direction.z);
            printf("Camera FOV: %f\n", scene->camera->fov);
        } else
        { 
            printf("Camera is NULL\n");
        }

        //  ambient
        printf("\n--- Ambient ---\n");
        if (scene->ambient)
        {
            printf("Ambient Ratio: %f\n", scene->ambient->ambient_ration);
            printf("Ambient Color: [%f, %f, %f]\n", scene->ambient->ambient_color.r, scene->ambient->ambient_color.g, scene->ambient->ambient_color.b);
        } else {
            printf("Ambient is NULL\n");
        }

        //  light
        printf("\n--- Light ---\n");
        if (scene->light) {
            while(scene->light)
            {
                printf("Light Coordinates: [%f, %f, %f]\n", scene->light->coordinate.x, scene->light->coordinate.y, scene->light->coordinate.z);
                printf("Light Ratio: %f\n", scene->light->ration);
                printf("Light Color: [%f, %f, %f]\n", scene->light->color.r, scene->light->color.g, scene->light->color.b);
                scene->light = scene->light->next;
            }
        } else {
            printf("Light is NULL\n");
        }

        //  spheres
        printf("\n--- Spheres ---\n");
        t_sphere *sphere = scene->sphere;
        if (sphere) {
            while (sphere) {
                printf("Sphere Coordinates: [%f, %f, %f]\n", sphere->sphere_coordinates.x, sphere->sphere_coordinates.y, sphere->sphere_coordinates.z);
                printf("Sphere Diameter: %f\n", sphere->sphere_diameter);
                printf("Sphere Color: [%f, %f, %f]\n", sphere->sphere_color.r, sphere->sphere_color.g, sphere->sphere_color.b);
                sphere = sphere->next;
                printf("================================\n");
            }
        } else {
            printf("No Spheres in Scene\n");
        }

        //  planes
        printf("\n--- Planes ---\n");
        t_plane *plane = scene->plane;
        if (plane) {
            while (plane) {
                printf("Plane Coordinates: [%f, %f, %f]\n", plane->plane_cordinates.x, plane->plane_cordinates.y, plane->plane_cordinates.z);
                printf("Plane Normal: [%f, %f, %f]\n", plane->plane_normal.x, plane->plane_normal.y, plane->plane_normal.z);
                printf("Plane Color: [%f, %f, %f]\n", plane->plane_color.r, plane->plane_color.g, plane->plane_color.b);
                plane = plane->next;
                printf("================================\n");

            }
        } else {
            printf("No Planes in Scene\n");
        }

        //  cylinders
        printf("\n--- Cylinders ---\n");
        t_cylinder *cylinder = scene->cylinder;
        if (cylinder) {
            while (cylinder) {
                printf("Cylinder Coordinates: [%f, %f, %f]\n", cylinder->coordinates.x, cylinder->coordinates.y, cylinder->coordinates.z);
                printf("Cylinder Orientation: [%f, %f, %f]\n", cylinder->orientation.x, cylinder->orientation.y, cylinder->orientation.z);
                printf("Cylinder raduis: %f, Height: %f\n", cylinder->raduis, cylinder->height);
                printf("Cylinder Color: [%f, %f, %f]\n", cylinder->cylinder_color.r, cylinder->cylinder_color.g, cylinder->cylinder_color.b);
                cylinder = cylinder->next;
                printf("================================\n");
            }
        } else {
            printf("No Cylinders in Scene\n");
        }
    }
    else {
        printf("Scene is NULL\n");
    }
}

t_map   *ft_init_map()
{
    t_map   *new;
    int i;

    i = -1;
    new = (t_map *)malloc(sizeof(t_map));
    if (!new)
        return (NULL);
    new->lines = NULL;
    while (++i < 3)
        new->scen_elements[i] = 0;    
    return (new);
}

int	ft_close(t_var *vars)
{
    printf("============> (hehooooooooooo) <============\n");
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0); // Exit the program
	return (0);
}



int	ft_close_window(t_scene *scen)
{
    ft_destroy_scene(scen);
    // ft_free_map_line(scen->map); 
    ft_free_map(&scen->map->lines);
    free(scen->map);  
    mlx_destroy_image(scen->data->mlx, scen->data->img.img_ptr);
    mlx_destroy_window(scen->data->mlx, scen->data->win);
    mlx_destroy_display(scen->data->mlx);
    free(scen->data->mlx);
    free(scen->data);
    free(scen);
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

void    init_scene(t_scene *scene)
{
    scene->data->mlx = mlx_init();
    scene->data->win = mlx_new_window(scene->data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT,"MiniRT");
    scene->data->img.img_ptr = mlx_new_image(scene->data->mlx,SCREEN_WIDTH, SCREEN_HEIGHT);
    scene->data->img.addr = mlx_get_data_addr(scene->data->img.img_ptr,
                                             &scene->data->img.bits_per_pixel,
                                             &scene->data->img.line_length,
                                             &scene->data->img.endian);
}

t_scene *allocate_scene(void)
{
    t_scene *scene;

    scene = malloc(sizeof(t_scene));
    if (!scene)
        return NULL;
    scene->data = malloc(sizeof(t_var));
    if (!scene->data)
        return NULL;
    scene->sphere = NULL;
    scene->plane = NULL;
    scene->cylinder = NULL;
    scene->cone = NULL;
    scene->light = NULL;
    return (scene);
}

void     ft_free_map_line(t_map *map)
{
    t_map_line *tmp;
    int  i;

    while(map->lines)
    {
        tmp =  map->lines;
        i = 0;
        while(map->lines->line_component[i])
        {
            free(map->lines->line_component[i]);
            i++;
        }
        free(map->lines);
        map->lines = tmp;
    }
}

/*main function*/
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
        return (free(map),ft_putstr_fd("map is empty\n", 2), 1);
    scene = allocate_scene();
    init_scene(scene);
    if (!ft_generate_scene(map->lines, &scene))
        return (0);
    scene->map = map;
    scene->world = default_world(scene);
    render_image(scene, scene->world, scene->camera);
    return (0);
}
