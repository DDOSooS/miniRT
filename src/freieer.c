/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freieer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:21:45 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:21:55 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minirt.h"

void ft_free_camera(t_scamera **cam)
{
    ft_free_matrix((*cam)->transform,4);
    free(*cam);
}

void ft_free_sphere(t_sphere *sphere, t_var *data)
{
    t_sphere *tmp;
    
    while(sphere)
    {
        tmp = sphere->next;
        ft_free_matrix(sphere->transform,4);
        if (sphere->has_texture)
        {
            mlx_destroy_image(data->mlx, sphere->texture->img_ptr);
            free(sphere->texture);
        }
        free(sphere->material);
        free(sphere);
        sphere = tmp;
    }
}

void ft_free_cylinder(t_cylinder *cy)
{
    t_cylinder *tmp;

    while(cy)
    {
        tmp = cy->next;
        ft_free_matrix(cy->transform, 4);
        free(cy->material);
        free(cy);
        cy = tmp;
    }
}

void ft_free_plane(t_plane *pl)
{
    t_plane *tmp;

    while (pl)
    {
        tmp = pl->next;
        ft_free_matrix(pl->transform, 4);
        free(pl->material);
        free(pl);
        pl = tmp;
    }
}

//to be adjusted with multiple spot lights
void ft_free_light(t_light *light)
{
    t_light *tmp;

    while(light)
    {
        tmp = light->next;
        free(light);
        light = tmp;
    }
}


void ft_free_cone(t_cone *cone)
{
    t_cone *tmp;

    while(cone)
    {
        tmp = cone->next;
        ft_free_matrix(cone->transform, 4);
        free(cone->material);
        free(cone);
        cone =  tmp;
    }
}

void ft_free_shapes(t_shape *shape)
{
    t_shape *tmp;

    while(shape)
    {
        tmp = shape->next;
        free(shape);
        shape = tmp;
    }
}

void ft_destroy_scene(t_scene *scen)
{
    free(scen->ambient);
    ft_free_light(scen->light); 
    ft_free_camera(&scen->camera);
    ft_free_plane(scen->plane);
    ft_free_sphere(scen->sphere, scen->data);
    ft_free_cylinder(scen->cylinder);
    ft_free_cone(scen->cone);
    ft_free_shapes(scen->world->shape);
    free(scen->world);
}