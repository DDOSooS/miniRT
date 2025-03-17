/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:50:18 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:19:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../includes/minirt.h"

void	ft_add_shape(t_world **root, void *new, int shape)
{
	t_shape	*current;

	if (!(*root)->shape)
	{
		(*root)->shape = ft_new_shape(new, shape);
		(*root)->n_objects++;
	}
	else
	{
		current = (*root)->shape;
		while (current->next)
			current = current->next;
		current->next = ft_new_shape(new, shape);
		(*root)->n_objects++;
	}
}

void	ft_add_sphere_shape(t_world *world, t_sphere *sphere)
{
	t_sphere	*tmp;

	tmp = sphere;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_SPHERE);
		tmp = tmp->next;
	}
}

void	ft_add_cone_shape(t_world *world, t_cone *cone)
{
	t_cone	*tmp;

	tmp = cone;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_CONE);
		tmp = tmp->next;
	}
}

void	ft_add_plight(t_world *world, t_light *light)
{
	t_light	*tmp;

	world->light = light;
	tmp = light;
	while (tmp)
	{
		tmp->color = ft_scale_color(tmp->color, tmp->ration);
		tmp = tmp->next;
	}
}

t_world	*default_world(t_scene *scene)
{
	t_world	*world;

	world = malloc(sizeof(t_world));
	if (!world)
		return (NULL);
	world->n_objects = 0;
	world->shape = NULL;
	if (scene->cylinder)
		ft_add_cylinder_shape(world, scene->cylinder);
	if (scene->plane)
		ft_add_plane_shape(world, scene->plane);
	if (scene->sphere)
		ft_add_sphere_shape(world, scene->sphere);
	if (scene->cone)
		ft_add_cone_shape(world, scene->cone);
	ft_add_plight(world, scene->light);
	return (world);
}
