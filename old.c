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
    // --- Dump t_scene ---
    if (scene)
    {
        // Dump camera
        printf("\n--- Camera  ---\n");
        if (scene->camera)
        {
            printf("Camera Position: [%f, %f, %f]\n", scene->camera->camera_position->x, scene->camera->camera_position->y, scene->camera->camera_position->z);
            printf("Camera Coordinates: [%f, %f, %f]\n", scene->camera->camera_dir->x, scene->camera->camera_dir->y, scene->camera->camera_dir->z);
            printf("Camera FOV: %d\n", scene->camera->camera_fov);
        } else {
            printf("Camera is NULL\n");
        }

        // Dump ambient
        printf("\n--- Ambient ---\n");
        if (scene->ambient)
        {
            printf("Ambient Ratio: %f\n", scene->ambient->ambient_ration);
            printf("Ambient Color: [%d, %d, %d]\n", scene->ambient->ambient_color->r, scene->ambient->ambient_color->g, scene->ambient->ambient_color->b);
        } else {
            printf("Ambient is NULL\n");
        }

        // Dump light
        printf("\n--- Light ---\n");
        if (scene->light) {
            printf("Light Coordinates: [%f, %f, %f]\n", scene->light->light_coordinate->x, scene->light->light_coordinate->y, scene->light->light_coordinate->z);
            printf("Light Ratio: %f\n", scene->light->light_ration);
            printf("Light Color: [%d, %d, %d]\n", scene->light->light_color->r, scene->light->light_color->g, scene->light->light_color->b);
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
                printf("Sphere Color: [%d, %d, %d]\n", sphere->sphere_color->r, sphere->sphere_color->g, sphere->sphere_color->b);
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
                printf("Plane Color: [%d, %d, %d]\n", plane->plane_color->r, plane->plane_color->g, plane->plane_color->b);
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
                printf("Cylinder raduis: %f, Height: %f\n", cylinder->raduis, cylinder->height);
                printf("Cylinder Color: [%d, %d, %d]\n", cylinder->cylinder_color->r, cylinder->cylinder_color->g, cylinder->cylinder_color->b);
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

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}


void my_pixel_put(t_img *img, int x, int y, int color)
{
    int offset;

    offset = (img->line_length * y) + (x * (img->bits_per_pixel / 8));
    *((unsigned int *)(img->addr + offset)) = color;
}

void var_dump_vector(t_vector *vec)
{
    printf("x-> (%f) y-> (%f) z-> (%f) ===\n",vec->x, vec->y, vec->z);
}

/*
// camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    
    ---- color generation ------------------

    color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);



}
*/

    // if (hit_sphere(point3(0,0, -1),0.5, r))
    //     return (255 << 24) | (255 << 16) | (0 << 8) | 0;


// int hit_sphere(t_vector *center, float radius, t_ray *ray) {
//     t_vector *disc = vector_sub(center, ray->direction);
//     float a = vector_dot(ray->direction, ray->direction);
//     float b = 2.0f * vector_dot(ray->direction, disc);
//     float c = vector_dot(disc, disc) - radius * radius;
//     float discriminant = b * b - 4 * a * c;
//     return (discriminant >= 0);
// }

// int ray_color(t_ray *r, t_scene *scene)
// {
//     t_vector *unit_dir;
//     float t;
    
//     if (hit_sphere(scene->sphere->sphere_coordinates, scene->sphere->sphere_diameter, r))
//         return ((255 << 24) | (255 << 16) | (0 << 8) | 0);
//     unit_dir = vector_normilze(r->direction);
//     if (!unit_dir)
//         return 0; 
        
//     t = 0.5 * (unit_dir->y + 1.0);
    
//     int r_color = (int)(((1.0 - t) * 1.0 + t * 0.5) * 255);
//     int g_color = (int)(((1.0 - t) * 1.0 + t * 0.7) * 255);
//     int b_color = (int)(((1.0 - t) * 1.0 + t * 1.0) * 255);
    
//     free(unit_dir);
//     return (255 << 24) | (r_color << 16) | (g_color << 8) | b_color;
// }

// void calculate_viewport_vectors(t_scene *scene)
// {
//     t_camera *tmp;
//     t_vector *camera_up;

//     tmp = scene->camera;
//     camera_up = ft_new_vector(0.0,0.0,1.0);
//     tmp->focal_lenght = 1.0;
//     tmp->hor_size = 0.25;
//     tmp->alignement_vector = vector_sub(tmp->camera_dir, tmp->camera_position);
//     tmp->alignement_vector = vector_normilze(tmp->alignement_vector);
//     tmp->cam_u = vector_cross(tmp->alignement_vector, camera_up);
//     tmp->cam_u = vector_normilze(tmp->cam_u);
//     tmp->cam_v = vector_cross(tmp->cam_u, tmp->alignement_vector);
//     tmp->cam_v = vector_normilze(tmp->cam_v);
//     tmp->projection_center = vector_add(tmp->camera_position,
//                                         vector_multiply_scalar(
//                                             tmp->alignement_vector,
//                                             tmp->focal_lenght)
//                                         );
//     tmp->cam_u = vector_multiply_scalar(tmp->cam_u, tmp->hor_size);
//     tmp->cam_v = vector_multiply_scalar(tmp->cam_v, tmp->hor_size / scene->aspect_ratio);
// }

// void render_scene(t_scene *scene)
// {
//     int         x, y;
//     t_vector    *inter_point;
//     t_vector    *local_normal;
//     t_vector    *local_color;
//     t_ray       *cam_ray;
//     float       x_fact;
//     float       y_fact;
//     int         color;


//     x_fact = 1.0 / ((float) scene->image_width / 2.0);
//     y_fact = 1.0 / ((float) scene->image_height / 2.0);
    
// /*
//     int         pixel_color;
//     t_vector    *pixel_center;
//     t_ray       *ray;



//     // Pre-calculate viewport dimensions
//     float viewport_width = 2.0f * tan(scene->camera->camera_fov * PI / 360.0f);
//     float viewport_height = viewport_width / scene->aspect_ratio;

//     for (y = 0; y < scene->image_height; y++)
//     {
//         for (x = 0; x < scene->image_width; x++)
//         {
//             // Transform screen coordinates to normalized device coordinates (-1 to 1)
//             float ndcX = (2.0f * x / (scene->image_width - 1)) - 1.0f;
//             float ndcY = 1.0f - (2.0f * y / (scene->image_height - 1)); // Flip Y coordinate

//             // Calculate the actual world space direction using NDC coordinates
//             t_vector *right_offset = vector_multiply_scalar(scene->camera->cam_u, ndcX * viewport_width * 0.5f);
//             t_vector *up_offset = vector_multiply_scalar(scene->camera->cam_v, ndcY * viewport_height * 0.5f);
            
//             // Calculate the point on the viewport in world space
//             t_vector *viewport_point = vector_add(scene->camera->projection_center, right_offset);
//             viewport_point = vector_add(viewport_point, up_offset);
            
//             // Calculate ray direction from camera position to viewport point
//             t_vector *dir = vector_sub(viewport_point, scene->camera->camera_position);
//             dir = vector_normilze(dir);
            
//             // Create and cast the ray
//             ray = create_ray(scene->camera->camera_position, dir);
//             pixel_color = ray_color(ray, scene);
//             my_pixel_put(&scene->data->img, x, y, pixel_color);
            
//             // Clean up
//             free(right_offset);
//             free(up_offset);
//             free(viewport_point);
//             free(dir);
//             free(ray);
//         }
//     }
//         printf("camera Screen Centre===\n");
//         var_dump_vector(scene->camera->projection_center);
//         printf("camera U vector===\n");
//         var_dump_vector(scene->camera->cam_u);
//         printf("camera V vector===\n");
//         var_dump_vector(scene->camera->cam_v);
// */
  
//     for(x =0 ; x< scene->image_width ; x++)
//     {
//         for ( y = 0; y < scene->image_height ; y++)
//         {
//             float    norm_x = (x * x_fact) - 1.0;
//             float    norm_y = (y * y_fact) - 1.0;
//             gen_ray(scene, norm_x, norm_y, &cam_ray);
//             if (check_intersection(cam_ray, inter_point, local_color, local_color))
//                 color = create_trgb(255, 255, 0, 0);
//             else
//                 color = create_trgb(255, 0, 0, 0);
//             my_pixel_put(&scene->data->img, x, y, color);
//         }
//     }

//     mlx_put_image_to_window(scene->data->mlx, scene->data->win, scene->data->img.img_ptr, 0, 0);
//     mlx_loop(scene->data->mlx);
// }

void init_scene(t_scene *scene) {
    scene->image_width = 1280;
    scene->aspect_ratio = 16.0f / 9.0f;
    scene->image_height = 720;
    
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

int intersection(t_ray *ray, t_sphere *sphere) {
    t_vector    *tmp ;

    tmp = vector_sub(ray->origin,sphere->sphere_coordinates);
    double a = 1.0;  // Assuming normalized ray direction
    double b = 2.0 * vector_dot(ray->direction, tmp);
    double c = vector_dot(tmp, tmp) - sphere->sphere_diameter * sphere->sphere_diameter;
    
    float delta = b * b - 4 * a * c;
    
    if (delta < 0)
        return 0;
    return (1);
    double t1 = (-b - sqrt(delta)) / (2 * a);
    double t2 = (-b + sqrt(delta)) / (2 * a);
    
    return (t2 > 0);
}

int render_sphere(t_scene *scene) {
    float fov = scene->camera->camera_fov * PI / 180.0f;
    
    for (int y = 0; y < scene->image_height; y++) {
        for (int x = 0; x < scene->image_width; x++) {
            t_vector *dir = ft_new_vector(
                (x - scene->image_width / 2.0f), 
                (y - scene->image_height / 2.0f), 
                -scene->image_width / (2.0f * tan(fov / 2.0f))
            );
            
            t_ray *ray = create_ray(scene->camera->camera_position, dir);
            
            int hit_color = create_trgb(255, 255, 0, 0);     // Red for hit
            int miss_color = create_trgb(255, 0, 0, 0);      // Black for miss
            
            if (intersection(ray, scene->sphere)) {
                my_pixel_put(&scene->data->img, x, y, hit_color);
            } else {
                my_pixel_put(&scene->data->img, x, y, miss_color);
            }
            
            // Free memory to prevent leaks
            free(dir);
            free(ray);
        }
    }
    
    mlx_put_image_to_window(scene->data->mlx, scene->data->win, scene->data->img.img_ptr, 0, 0);
    mlx_loop(scene->data->mlx);
    
    return 1;
}

int main(int argc, char **argv)
{
    t_map   *map;
    t_scene *scene;

    if (argc != 2)
        return (printf("error: try with : ./minirt scene.rt\n"), 1);
    map = ft_init_map();
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_scen_map(argv[1]);
    if (!map->lines || !ft_check_map_components(&map))
        return (free(map),ft_putstr_fd("map is empty\n",2),1);
    scene = ft_generate_scene(map->lines);
    var_dump_all(map, scene);
    init_scene(scene);

    render_sphere(scene);
    return (0);
}























/*



// // void	color_screen(t_var *data, int color)
// // {
// // 			my_pixel_put(&data->img,
// // 						X,
// // 						Y,
// // 						color);
// // }
// // int	handle_input(int keysym, t_var *data)
// // {
// //     //Check the #defines
// //     //find / -name keysym.h 2>/dev/null
// //     //find / -name keysymdef.h 2>/dev/null
// //     if (keysym == XK_escape)
// //     {
// //         printf("The %d key (ESC) has been pressed\n\n", keysym);
// //         mlx_destroy_window(data->mlx_ptr, data->win_ptr);
// //         mlx_destroy_display(data->mlx_ptr);
// //         free(data->mlx_ptr);
// //         exit(1);
// //     }
// //     printf("The %d key has been pressed\n\n", keysym);
// //     return (0);
// // }


// // int f(int keysym, t_var *data)
// // {
// //     printf("Pressed %d\n", keysym);
// //     //sleep to appreciate loop_hook stopping
// //     sleep(1);
// //     return 1;
// // }



// // int     change_color(t_var *data)
// // {
// //     // Fill the window with the current color
// // //  mlx_clear_window(data->mlx, data->win);
// //     mlx_string_put(data->mlx_ptr, data->win_ptr, 150, 150, data->color, "Color Changing Window!");

// //     // Cycle through some basic colors: RED, GREEN, BLUE
// //     if (data->color == 0xFF0000)        // If it's red
// //         data->color = 0x00FF00;        // Change to green
// //     else if (data->color == 0x00FF00)   // If it's green
// //         data->color = 0x0000FF;        // Change to blue
// //     else
// //         data->color = 0xFF0000;        // Otherwise, go back to red

// //     return (0);
// // }

// void	color_screen(t_var *data);

// t_vector *ft_new_vector(float x, float y, float z)
// {
//     t_vector *new;

//     new = malloc(sizeof(t_vector));
//     if (!new)
//         return NULL;
//     new->x = x;// int ray_color(t_ray *r) 
// {
//     t_vector *unit_dir;
//     float a;
//     int r_color, g_color, b_color;
    
//     if (hit_sphere(point3(0,0, -1),0.5, r))
//         return (255 << 24) | (255 << 16) | (0 << 8) | 0;
//     unit_dir = unit_vector(r->direction);
//     if (!unit_dir)
//         return 0; 
        
//     a = 0.5 * (unit_dir->y + 1.0);
    
//     r_color = (int)(((1.0 - a) * 1.0 + a * 0.5) * 255);
//     g_color = (int)(((1.0 - a) * 1.0 + a * 0.7) * 255);
//     b_color = (int)(((1.0 - a) * 1.0 + a * 1.0) * 255);
//     free(unit_dir);
//     return (255 << 24) | (r_color << 16) | (g_color << 8) | b_color;
// }

//     new->y = y;
//     new->z = z;
//     return new;
// }

// t_vector *point3(float x, float y, float z)
// {
//     t_vector *new;

//     new = malloc(sizeof(t_vector));
//     if (!new)
//         return NULL;
//     new->x = x;
//     new->y = y;
//     new->z = z;
//     return new;
// }

// t_vector    *ft_subtract_vect(t_vector *v1, t_vector *v2)
// {
//     t_vector *sub_vec;

//     sub_vec = point3(0,0,0);
//     if (!sub_vec)
//         return  NULL;
//     sub_vec->x = v1->x - v2->x;
//     sub_vec->y = v1->y - v2->y;
//     sub_vec->z = v1->z - v2->z;
//     return (sub_vec);
// }

// t_vector *ft_scale_vec(t_vector *vec, float scalar)
// {
//     t_vector *new = point3(0,0,0);
Ce type d’information doit donner une idée du niveau d’activité sur votre futur site, ce qui
peut impacter le choix de l’infrastructure technique.
//     new->z = v1->z + v2->z;
//     return new;
// }
// typedef struct s_ray
// {
//     t_vector *origin;
//     t_vector *direction;
// }   t_ray;

// int	create_trgb(int t, int r, int g, int b)
// {
// 	return (t << 24 | r << 16 | g << 8 | b);
// }

// float  ft_vect_magnitude(t_vector *v)
// {
//     return (sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z , 2)));
// }

// float dot_product(t_vector *a, t_vector *b)
// {
//     return a->x * b->x + a->y * b->y + a->z * b->z;
// }


// t_vector *unit_vector(t_vector *vec)
// {
//     t_vector *new;

//     float magnitude = ft_vect_magnitude(vec);
//     if (!magnitude || !vec)
//         return (NULL);
//     new = ft_new_vector(0,0,0);
//     new->x = vec->x / magnitude;
//     new->y = vec->y / magnitude;
//     new->z = vec->z / magnitude;
//     return (new);
// }

// int hit_sphere(t_vector *center, float radius, t_ray *r)
// {
//     t_vector *oc = ft_subtract_vect(center , r->origin);
//     float a = dot_product(r->direction, r->direction);
//     float b = -2.0 * dot_product(r->direction, oc);
//     float c = dot_product(oc, oc) - radius*radius;
//     float discriminant = b*b - 4*a*c;
//     printf("discriminant=== %f==\n", discriminant);
//     return (discriminant >= 0);
// }

// int ray_color(t_ray *r) 
// {
//     t_vector *unit_dir;
//     float a;
//     int r_color, g_color, b_color;
    
//     if (hit_sphere(point3(0,0, -1),0.5, r))
//         return (255 << 24) | (255 << 16) | (0 << 8) | 0;
//     unit_dir = unit_vector(r->direction);
//     if (!unit_dir)
//         return 0; 
        
//     a = 0.5 * (unit_dir->y + 1.0);
    
//     r_color = (int)(((1.0 - a) * 1.0 + a * 0.5) * 255);
//     g_color = (int)(((1.0 - a) * 1.0 + a * 0.7) * 255);
//     b_color = (int)(((1.0 - a) * 1.0 + a * 1.0) * 255);
//     free(unit_dir);
//     return (255 << 24) | (r_color << 16) | (g_color << 8) | b_color;
// }

// t_ray *ft_new_ray(t_vector *origin, t_vector *direction)
// {
//     t_ray *new;

//     new = malloc(sizeof(t_ray));
//     if (!new)
//         return (NULL);
//     new->origin = origin;
//     new->direction = direction;
//     return (new);
// }

// void	my_pixel_put(t_img *img, int x, int y, int color)
// {
// 	int	offset;

// 	offset = (img->line_len * y) + (x * (img->bits_per_pixel / 8)) ;	
// 	*((unsigned int *)(offset + img->img_pixels_ptr)) = color;
// }


// int	f(int keysym, t_var *data)
// {

// 	if (keysym == L_key)
// 	{
// 		color_screen(data);
// 	}
// 	else if (keysym == R_key)
// 	{
// 		color_screen(data);
// 	}
// 	else if (keysym == D_key)
// 	{
// 		color_screen(data);
// 	}	
//     else if (keysym == 2)
//         color_screen(data);
// 	else if (keysym == XK_escape)
// 		exit(1);
// 	mlx_put_image_to_window(data->mlx,
// 							data->win, 
// 							data->img.img_ptr, 
// 							0, 0);

// 	return 0;
// }

// void var_dump_vector(t_vector *v)
// {
//     printf("vector ==> x:(%f) ===> y:(%f)  ===> z:(%f)====\n" , v->x, v->y, v->z);
// }

 
// void	color_screen(t_var *data)
// {
//     float   aspect_ratio =  16.0 / 9.0;
//     float     image_width = 1000.0;

//     float image_height = (float)(image_width / aspect_ratio);

//     float focal_length = 4;
//     float viewport_height = 2.0;
//     float viewport_width = viewport_height * ((float)(image_width)/image_height);

//     // printf("W-%f--H:%f===\n", image_width, image_height);

//     // Create vectors
//     t_vector *camera_center = point3(0, 0, 0);
//     t_vector *viewport_u = point3(viewport_width, 0, 0);
//     t_vector *viewport_v = point3(0, -viewport_height, 0);

//     // Fix the scaling calculation by explicitly using float division
//     float scale_u = 1.0f / (float)image_width;
//     float scale_v = 1.0f / (float)image_height;

//     // Calculate pixel deltas
//     t_vector *pixel_delta_u = ft_scale_vec(viewport_u, scale_u);
//     t_vector *pixel_delta_v = ft_scale_vec(viewport_v, scale_v);

//     t_vector *tmp1 =  ft_scale_vec(viewport_u , 0.5);   
//     t_vector *tmp2 =  ft_scale_vec(viewport_v , 0.5);
//     t_vector *tmp3 = ft_subtract_vect(tmp1, tmp2);
//     t_vector *distance = point3(0,0,focal_length);
//     t_vector *viewport_upper_left  = ft_subtract_vect(camera_center, ft_subtract_vect(distance, tmp3));
//     t_vector *tmp4 = ft_vec_addition(pixel_delta_u , pixel_delta_v);
//     t_vector *tmp5 = ft_scale_vec(tmp4, 0.5);
//     t_vector *pixel_00 =  ft_vec_addition(viewport_upper_left , tmp5);

//     for (int y = 0; y < WINDOW_HEIGHT; ++y)	
// 	{
// 		for (int x = 0; x < WINDOW_WIDTH; ++x)
// 		{
//             t_vector * u_offset = ft_scale_vec(pixel_delta_u, x);
//             t_vector * v_offset = ft_scale_vec(pixel_delta_v, y);
//             t_vector *pixel_center = ft_vec_addition
//                 (   
//                     pixel_00, 
//                     ft_vec_addition(u_offset, v_offset)
//                 );
//             t_vector *ray_direction = ft_subtract_vect(pixel_center , camera_center);
//             t_ray *r = ft_new_ray(camera_center, ray_direction);
//             int  pixel_color = ray_color(r);
//             my_pixel_put(&data->img, x, y, pixel_color);
// 		}
// 	}
// }


// int main(int argc, char **argv)
// {
//     t_map   *map;
//     t_scene *scene;
//     // void *mlx;
//     // void *mlx_window;

//     if (argc != 2)
//         return (printf("error: try with : ./minirt scene.rt\n"), 1);
//     map = ft_init_map();
//     if (!map)
//         return (printf("error : memory allocation failed\n"), 1);
//     map->lines = ft_gen_scen_map(argv[1]);
//     if (!map->lines || !ft_check_map_components(&map))
//         return (free(map),ft_putstr_fd("map is empty\n",2),1);
//     scene = ft_generate_scene(map->lines);
//     var_dump_lines(map->lines);
//     var_dump_all(map, scene);
//     t_var	vars;

// 	vars.mlx = mlx_init();
// 	vars.win = mlx_new_window(vars.mlx,
// 								WINDOW_WIDTH, 
// 								WINDOW_HEIGHT, 
// 								"My window");
	

// 	// Code to create an image and get the related DATA
// 	vars.img.img_ptr = mlx_new_image(vars.mlx,
// 									WINDOW_WIDTH, 
// 									WINDOW_HEIGHT);
// 	vars.img.img_pixels_ptr = mlx_get_data_addr(vars.img.img_ptr, 
// 												&vars.img.bits_per_pixel, 
// 												&vars.img.line_len, 
// 												&vars.img.endian);
// 	mlx_key_hook(vars.win, 
// 				f,
// 				&vars);
//     // mlx_key_hook(vars.win, render_view_port, &vars);
// 	mlx_loop(vars.mlx);

//     return (0);
//     // var_dump_all(map , scene);
// } 
*/