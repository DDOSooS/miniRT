/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spher.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:35 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:10:43 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minirt.h"

t_sphere *ft_new_sphere(char **components, t_scene **scene)
{
	t_sphere *sphere;
	
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return NULL;
	ft_gen_elements(&sphere->sphere_coordinates, components[1]);
	sphere->sphere_diameter = ft_atod(components[2]);
	ft_gen_colors(&sphere->sphere_color, components[3]);
	sphere->transform = identity_matrix(4);
	sphere->material = default_material();
	ft_gen_colors(&sphere->material->color, components[3]);
	sphere->next = NULL;
	if (ft_count_components(components) == 6)
	{
		sphere->has_checkered = 1;
		sphere->has_texture = 0;
		ft_gen_colors(&sphere->checkered_color, components[5]);
	}
	else if (ft_count_components(components) == 5)
	{
		sphere->has_checkered = 0;
		sphere->has_texture = 1;
		if (!get_texture(sphere, components[4], scene))
		{
			ft_free_matrix(sphere->transform, 4);
			free(sphere->material);
			free(sphere);
			return NULL;
		}
	}
	else
	{
		sphere->has_texture = 0;
		sphere->has_checkered = 0;
	}
	return sphere;
}

int ft_add_sphere(t_scene **scene, char **components)
{
	t_sphere *sphere;
	t_sphere *tmp;
	
	sphere = ft_new_sphere(components, scene);
	if (!sphere)
			return 0;
	tmp = (*scene)->sphere;
	if (!tmp) 
		(*scene)->sphere = sphere;
	else
	{
		while (tmp->next)
			tmp =tmp->next;
		tmp->next = sphere;
	}
	return 1;
}