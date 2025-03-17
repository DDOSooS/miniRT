/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_shapes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:18:29 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 06:09:56 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_cone	*ft_new_cone(char **components)
{
	t_cone	*cone;

	cone = malloc(sizeof(t_cone));
	if (!cone)
		return (NULL);
	ft_gen_elements(&cone->apex, components[1]);
	ft_gen_elements(&cone->axis, components[2]);
	cone->height = ft_atod(components[3]);
	cone->radius = ft_atod(components[4]);
	ft_gen_colors(&cone->color, components[5]);
	cone->material = default_material();
	cone->transform = identity_matrix(4);
	cone->transform[0][3] = cone->apex.x;
	cone->transform[1][3] = cone->apex.y;
	cone->transform[2][3] = cone->apex.z;
	cone->next = NULL;
	cone->material->color = cone->color;
	if (vec_lenght(cone->axis) == 0)
	{
		ft_free_matrix(cone->transform, 4);
		free(cone->material);
		free(cone);
		return (NULL);
	}
	return (cone);
}

int	ft_add_cone(t_scene **scene, char **components)
{
	t_cone	*cone;
	t_cone	*tmp;

	cone = ft_new_cone(components);
	if (!cone)
		return (0);
	tmp = (*scene)->cone;
	if (!tmp)
		(*scene)->cone = cone;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cone;
	}
	return (1);
}

t_sphere	*init_sphere(char **components)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (NULL);
	ft_gen_elements(&sphere->sphere_coordinates, components[1]);
	sphere->sphere_diameter = ft_atod(components[2]);
	ft_gen_colors(&sphere->sphere_color, components[3]);
	sphere->transform = identity_matrix(4);
	sphere->material = default_material();
	ft_gen_colors(&sphere->material->color, components[3]);
	sphere->next = NULL;
	sphere->has_checkered = 0;
	sphere->has_texture = 0;
	return (sphere);
}

t_sphere	*ft_new_sphere(char **components, t_scene **scene)
{
	t_sphere	*sphere;
	int			comp_count;

	sphere = init_sphere(components);
	if (!sphere)
		return (NULL);
	comp_count = ft_count_components(components);
	if (!handle_sphere_texture(sphere, components, comp_count, scene))
		return (NULL);
	return (sphere);
}

int	ft_add_sphere(t_scene **scene, char **components)
{
	t_sphere	*sphere;
	t_sphere	*tmp;

	sphere = ft_new_sphere(components, scene);
	if (!sphere)
		return (0);
	tmp = (*scene)->sphere;
	if (!tmp)
		(*scene)->sphere = sphere;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = sphere;
	}
	return (1);
}
