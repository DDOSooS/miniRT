/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:05 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/29 18:02:43 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "./includes/minirt.h"

void var_dump_all(t_map *map, t_scene *scene)
{
    // --- Dump t_map ---
    if (map) {
        printf("\n--- Map ---\n");
        printf("Scen Elements: [%d, %d, %d]\n", map->scen_elements[0], map->scen_elements[1], map->scen_elements[2]);

        map_line *line = map->lines;
        while (line) {
            printf("Line Components:\n");
            for (int i = 0; line->line_component && line->line_component[i]; i++) {
                printf("  Component %d: %s\n", i, line->line_component[i]);
            }
            line = line->next;
        }
    } else {
        printf("Map is NULL\n");
    }

    // --- Dump t_scene ---
    if (scene) {
        // Dump camera
        printf("\n--- Camera %p ---\n", scene->camera);
        if (scene->camera) {
            printf("Camera Position: [%f, %f, %f]\n", scene->camera->camera_position[0], scene->camera->camera_position[1], scene->camera->camera_position[2]);
            printf("Camera Coordinates: [%f, %f, %f]\n", scene->camera->camera_cordinates[0], scene->camera->camera_cordinates[1], scene->camera->camera_cordinates[2]);
            printf("Camera FOV: %d\n", scene->camera->camera_fow);
        } else {
            printf("Camera is NULL\n");
        }

        // Dump ambient
        printf("\n--- Ambient ---\n");
        if (scene->ambient) {
            printf("Ambient Ratio: %f\n", scene->ambient->ambient_ration);
            printf("Ambient Color: [%d, %d, %d]\n", scene->ambient->ambient_color[0], scene->ambient->ambient_color[1], scene->ambient->ambient_color[2]);
        } else {
            printf("Ambient is NULL\n");
        }

        // Dump light
        printf("\n--- Light ---\n");
        if (scene->light) {
            printf("Light Coordinates: [%f, %f, %f]\n", scene->light->light_coordinate[0], scene->light->light_coordinate[1], scene->light->light_coordinate[2]);
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
                printf("Sphere Coordinates: [%f, %f, %f]\n", sphere->sphere_coordinates[0], sphere->sphere_coordinates[1], sphere->sphere_coordinates[2]);
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
                printf("Plane Coordinates: [%f, %f, %f]\n", plane->plane_cordinates[0], plane->plane_cordinates[1], plane->plane_cordinates[2]);
                printf("Plane Normal: [%f, %f, %f]\n", plane->plane_normal[0], plane->plane_normal[1], plane->plane_normal[2]);
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
                printf("Cylinder Coordinates: [%f, %f, %f]\n", cylinder->coordinates[0], cylinder->coordinates[1], cylinder->coordinates[2]);
                printf("Cylinder Orientation: [%f, %f, %f]\n", cylinder->orientation[0], cylinder->orientation[1], cylinder->orientation[2]);
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

int main(int argc, char **argv)
{
    t_map   *map;
    t_scene *scene;
    void *mlx;
    void *mlx_window;

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
    mlx = mlx_init();
    mlx_window = mlx_new_window(mlx, 500, 500, "MiniRT");
    
	mlx_loop(mlx);
    return (0);
    // var_dump_all(map , scene);
}