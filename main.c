/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:05 by aghergho          #+#    #+#             */
/*   Updated: 2024/11/01 10:20:23 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "./includes/minirt.h"
#define XK_escape 65307
#define L_key 65361
#define U_key 65362
#define R_key 65363
#define D_key 65364

void var_dump_all(t_map *map, t_scene *scene)
{
    // --- Dump t_map ---
    // if (map) {
    //     printf("\n--- Map ---\n");
    //     printf("Scen Elements: [%d, %d, %d]\n", map->scen_elements[0], map->scen_elements[1], map->scen_elements[2]);

    //     map_line *line = map->lines;
    //     while (line) {
    //         printf("Line Components:\n");
    //         for (int i = 0; line->line_component && line->line_component[i]; i++) {
    //             printf("  Component %d: %s\n", i, line->line_component[i]);
    //         }
    //         line = line->next;
    //     }
    // } else {
    //     printf("Map is NULL\n");
    // }

    // --- Dump t_scene ---
    if (scene)
    {
        // Dump camera
        printf("\n--- Camera  ---\n");
        if (scene->camera)
        {
            printf("Camera Position: [%f, %f, %f]\n", scene->camera->camera_position->x, scene->camera->camera_position->y, scene->camera->camera_position->z);
            printf("Camera Coordinates: [%f, %f, %f]\n", scene->camera->camera_cordinates->x, scene->camera->camera_cordinates->y, scene->camera->camera_cordinates->z);
            printf("Camera FOV: %d\n", scene->camera->camera_fow);
        } else {
            printf("Camera is NULL\n");
        }

        // Dump ambient
        printf("\n--- Ambient ---\n");
        if (scene->ambient)
        {
            printf("Ambient Ratio: %f\n", scene->ambient->ambient_ration);
            printf("Ambient Color: [%d, %d, %d]\n", scene->ambient->ambient_color[0], scene->ambient->ambient_color[1], scene->ambient->ambient_color[2]);
        } else {
            printf("Ambient is NULL\n");
        }

        // Dump light
        printf("\n--- Light ---\n");
        if (scene->light) {
            printf("Light Coordinates: [%f, %f, %f]\n", scene->light->light_coordinate->x, scene->light->light_coordinate->y, scene->light->light_coordinate->z);
            printf("Light Ratio: %f\n", scene->light->light_ration);
            printf("Light Color: [%d, %d, %d]\n", scene->light->light_color[0], scene->light->light_color[1], scene->light->light_color[2]);
        } else {
            printf("Light is NULL\n");
        }

        // Dump spheres
        printf("\n--- Spheres ---\n");
        t_sphere *sphere = scene->sphere;
        if (sphere) {
            while (sphere) {
                printf("Sphere Coordinates: [%f, %f, %f]\n", sphere->sphere_coordinates->x, sphere->sphere_coordinates->y, sphere->sphere_coordinates->z);
                printf("Sphere Diameter: %f\n", sphere->sphere_diameter);
                printf("Sphere Color: [%d, %d, %d]\n", sphere->sphere_color[0], sphere->sphere_color[1], sphere->sphere_color[2]);
                sphere = sphere->next;
            }
        } else {
            printf("No Spheres in Scene\n");
        }

        // Dump planes
        printf("\n--- Planes ---\n");
        t_plane *plane = scene->plane;
        if (plane) {
            while (plane) {
                printf("Plane Coordinates: [%f, %f, %f]\n", plane->plane_cordinates->x, plane->plane_cordinates->y, plane->plane_cordinates->z);
                printf("Plane Normal: [%f, %f, %f]\n", plane->plane_normal->x, plane->plane_normal->y, plane->plane_normal->z);
                printf("Plane Color: [%d, %d, %d]\n", plane->plane_color[0], plane->plane_color[1], plane->plane_color[2]);
                plane = plane->next;
            }
        } else {
            printf("No Planes in Scene\n");
        }

        // Dump cylinders
        printf("\n--- Cylinders ---\n");
        t_cylinder *cylinder = scene->cylinder;
        if (cylinder) {
            while (cylinder) {
                printf("Cylinder Coordinates: [%f, %f, %f]\n", cylinder->coordinates->x, cylinder->coordinates->y, cylinder->coordinates->z);
                printf("Cylinder Orientation: [%f, %f, %f]\n", cylinder->orientation->x, cylinder->orientation->y, cylinder->orientation->z);
                printf("Cylinder Diameter: %f, Height: %f\n", cylinder->diameter, cylinder->height);
                printf("Cylinder Color: [%d, %d, %d]\n", cylinder->colors[0], cylinder->colors[1], cylinder->colors[2]);
                cylinder = cylinder->next;
            }
        } else {
            printf("No Cylinders in Scene\n");
        }
    } else {
        printf("Scene is NULL\n");
    }
}

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



// void	color_screen(t_var *data, int color)
// {
// 			my_pixel_put(&data->img,
// 						X,
// 						Y,
// 						color);
// }
// int	handle_input(int keysym, t_var *data)
// {
//     //Check the #defines
//     //find / -name keysym.h 2>/dev/null
//     //find / -name keysymdef.h 2>/dev/null
//     if (keysym == XK_escape)
//     {
//         printf("The %d key (ESC) has been pressed\n\n", keysym);
//         mlx_destroy_window(data->mlx_ptr, data->win_ptr);
//         mlx_destroy_display(data->mlx_ptr);
//         free(data->mlx_ptr);
//         exit(1);
//     }
//     printf("The %d key has been pressed\n\n", keysym);
//     return (0);
// }


// int f(int keysym, t_var *data)
// {
//     printf("Pressed %d\n", keysym);
//     //sleep to appreciate loop_hook stopping
//     sleep(1);
//     return 1;
// }



// int     change_color(t_var *data)
// {
//     // Fill the window with the current color
// //  mlx_clear_window(data->mlx, data->win);
//     mlx_string_put(data->mlx_ptr, data->win_ptr, 150, 150, data->color, "Color Changing Window!");

//     // Cycle through some basic colors: RED, GREEN, BLUE
//     if (data->color == 0xFF0000)        // If it's red
//         data->color = 0x00FF00;        // Change to green
//     else if (data->color == 0x00FF00)   // If it's green
//         data->color = 0x0000FF;        // Change to blue
//     else
//         data->color = 0xFF0000;        // Otherwise, go back to red

//     return (0);
// }

void	color_screen(t_var *data);

t_vector *point3(float x, float y, float z)
{
    t_vector *new;

    new = malloc(sizeof(t_vector));
    if (!new)
        return NULL;
    new->x = x;
    new->y = y;
    new->z = z;
    return new;
}

t_vector    *ft_subtract_vect(t_vector *v1, t_vector *v2)
{
    t_vector *sub_vec;

    sub_vec = ft_new_vector(0,0,0);
    if (!sub_vec)
        return  NULL;
    sub_vec->x = v1->x - v2->x;
    sub_vec->y = v1->y - v2->y;
    sub_vec->z = v1->z - v2->z;
    return (sub_vec);
}

t_vector *ft_scale_vec(t_vector *vec, float scalar)
{
    t_vector *new = point3(0,0,0);

    new->x = (float) vec->x *scalar;
    new->y = (float) vec->y *scalar;
    new->z = (float) vec->z *scalar;
    return (new);
}

t_vector *ft_vec_addition(t_vector *v1, t_vector *v2)
{
    t_vector *new = point3(0,0,0);

    new->x = v1->x + v2->x;
    new->y = v1->y + v2->y;
    new->z = v1->z + v2->z;
    return new;
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

float  ft_vect_magnitude(t_vector *v)
{
    return (sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z , 2)));
}

float dot_product(t_vector *a, t_vector *b)
{
    return a->x * b->x + a->y * b->y + a->z * b->z;
}

typedef struct s_ray
{
    t_vector *origin;
    t_vector *direction;
}   t_ray;



int ray_color(t_ray *r)
{
    // Normalize the direction vector
    float length = ft_vect_magnitude(r->direction);
    t_vector *unit_direction = ft_scale_vec(r->direction, 1.0f/length);
    
    // Map direction to color (simple example)
    int red = (int)(255.999 * (unit_direction->x + 1.0) * 0.5);
    int green = (int)(255.999 * (unit_direction->y + 1.0) * 0.5);
    int blue = (int)(255.999 * (unit_direction->z + 1.0) * 0.5);
    
    free(unit_direction);
    return create_trgb(255, red, green, blue);
}

t_ray *ft_new_ray(t_vector *origin, t_vector *direction)
{
    t_ray *new;

    new = malloc(sizeof(t_ray));
    if (!new)
        return (NULL);
    new->origin = origin;
    new->direction = direction;
    return (new);
}

void	my_pixel_put(t_img *img, int x, int y, int color)
{
	int	offset;

	//🚨 Line len is in bytes. WIDTH 800 len_line ~3200 (can differ for alignment)
	offset = (img->line_len * y) + (x * (img->bits_per_pixel / 8)) ;	

	*((unsigned int *)(offset + img->img_pixels_ptr)) = color;
}


int	f(int keysym, t_var *data)
{

	if (keysym == L_key)
	{
		color_screen(data);
	}
	else if (keysym == R_key)
	{
		color_screen(data);
	}
	else if (keysym == D_key)
	{
		color_screen(data);
	}	
    else if (keysym == 2)
        color_screen(data);
	else if (keysym == XK_escape)
		exit(1);

	// push the READY image to window
	// the last parameters are the offset image-window
	mlx_put_image_to_window(data->mlx,
							data->win, 
							data->img.img_ptr, 
							0, 0);

	return 0;
}



void	color_screen(t_var *data)
{
    float   aspect_ratio =  16.0 / 9.0;
    int     image_width = 400.0;
    int color;

    int image_height = (int)(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;
    float focal_length = 1.0;
    float viewport_height = 2.0;
    float viewport_width = viewport_height * ((float)(image_width)/image_height);
    t_vector *camera_center = point3(0, 0, 0);
    t_vector *viewport_u = point3(viewport_width, 0, 0);
    t_vector *viewport_v = point3(0, -viewport_height, 0);
    t_vector *pixel_delta_u = ft_scale_vec(viewport_u , 1/ image_width);
    t_vector *pixel_delta_v = ft_scale_vec(viewport_v, 1/ image_height);

    // auto viewport_upper_left = camera_center
    //                      - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    
    t_vector *tmp1 =  ft_scale_vec(viewport_u , 0.5);   
    t_vector *tmp2 =  ft_scale_vec(viewport_v , 0.5);
    t_vector *tmp3 = ft_subtract_vect(tmp1, tmp3);
    t_vector *distance = point3(0,0,focal_length);
    t_vector *viewport_upper_left  = ft_subtract_vect(camera_center, ft_subtract_vect(distance, tmp3));
    t_vector *tmp4 = ft_vec_addition(pixel_delta_u , pixel_delta_v);
    t_vector *tmp5 = ft_scale_vec(tmp4, 0.5);
    t_vector *pixel_00 =  ft_vec_addition(viewport_upper_left , tmp5);

	// for (int y = 0; y < WINDOW_HEIGHT; ++y)	
	// {
	// 	for (int x = 0; x < WINDOW_WIDTH; ++x)
	// 	{
    //         int red = (int)(255.0 * (x / (float)(WINDOW_WIDTH - 1)));
    //         int green = (int)(255.0 * (y / (float)(WINDOW_HEIGHT - 1)));
    //         int blue = 0;  // Set blue to 0 or adjust as needed
    //         color = create_trgb(255, red, green, blue);  // 255 for full opacity
            
    //         my_pixel_put(&data->img, x, y, color);
	// 	}
	// }	
    for (int y = 0; y < WINDOW_HEIGHT; ++y)	
	{
		for (int x = 0; x < WINDOW_WIDTH; ++x)
		{

            t_vector *pixel_center = ft_vec_addition(pixel_00, ft_vec_addition(ft_scale_vec(pixel_delta_u, y), ft_scale_vec(pixel_delta_v, x)));

            t_vector *ray_direction = ft_subtract_vect(pixel_center , camera_center);
            t_ray *r = ft_new_ray(camera_center, ray_direction);

            int  pixel_color = ray_color(r);
            my_pixel_put(&data->img, x, y, pixel_color);
		}
	}
}


// float ft_get_vp_with(t_camera *cam)
// {
//     t_vector    *distance;
//     float       distance_lenght;
//     t_vector    *vp_origin;
//     t_vector    *cam

//     vp_origin = ft_new_vector(0,0,0);
//     distance = ft_subtract_vect(cam)

// }

// void render_view_port(int keystrm, t_var *data)
// {
//     float vp_with;

//     vp_with = ft_get_vp_with(data->scene->camera);
//     float min_x = -vp_with /2;
//     float max_x = vp_with /2;

//     for (float y = 0; y < WINDOW_HEIGHT ; ++y)	
// 	{
// 		for (float x = 0; x < WINDOW_WIDTH; ++x)
// 		{
//             if (x >= min_x && x <= max_x)
//                 my_pixel_put(&data->img,
// 				    		x, 
// 				    		y, 
// 						    0xff00);
// 		}
// 	}
// }

int main(int argc, char **argv)
{
    t_map   *map;
    t_scene *scene;
    // void *mlx;
    // void *mlx_window;

    if (argc != 2)
        return (printf("error: try with : ./minirt scene.rt\n"), 1);
    map = ft_init_map();
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_scen_map(argv[1]);
    if (!map->lines || !ft_check_map_components(&map))
        return (free(map),ft_putstr_fd("map is empty\n",2),1);
    scene = ft_generate_scene(map->lines);
    var_dump_lines(map->lines);
    var_dump_all(map, scene);
    t_var	vars;

	vars.mlx = mlx_init();
	vars.win = mlx_new_window(vars.mlx,
								WINDOW_WIDTH, 
								WINDOW_HEIGHT, 
								"My window");
	

	// Code to create an image and get the related DATA
	vars.img.img_ptr = mlx_new_image(vars.mlx,
									WINDOW_WIDTH, 
									WINDOW_HEIGHT);
	vars.img.img_pixels_ptr = mlx_get_data_addr(vars.img.img_ptr, 
												&vars.img.bits_per_pixel, 
												&vars.img.line_len, 
												&vars.img.endian);
	mlx_key_hook(vars.win, 
				f,
				&vars);
    // mlx_key_hook(vars.win, render_view_port, &vars);
	mlx_loop(vars.mlx);

    return (0);
    // var_dump_all(map , scene);
} 