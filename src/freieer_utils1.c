/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freieer_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:41:23 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:31:05 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

void	ft_free_camera(t_scamera **cam)
{
	ft_free_matrix((*cam)->transform, 4);
	free(*cam);
}

void	ft_free_sphere(t_sphere *sphere, t_var *data)
{
	t_sphere	*tmp;

	while (sphere)
	{
		tmp = sphere->next;
		ft_free_matrix(sphere->transform, 4);
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

void	ft_free_cylinder(t_cylinder *cy)
{
	t_cylinder	*tmp;

	while (cy)
	{
		tmp = cy->next;
		ft_free_matrix(cy->transform, 4);
		free(cy->material);
		free(cy);
		cy = tmp;
	}
}

void	ft_free_map_line(t_map *map)
{
	t_map_line	*tmp;
	int			i;

	while (map->lines)
	{
		tmp = map->lines;
		i = 0;
		while (map->lines->line_component[i])
		{
			free(map->lines->line_component[i]);
			i++;
		}
		free(map->lines);
		map->lines = tmp;
	}
}
