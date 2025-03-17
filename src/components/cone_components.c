/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_components.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:13 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:10:18 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minirt.h"

t_cone  *ft_new_cone(char **components)
{
	t_cone  *cone;
	
	cone = malloc(sizeof(t_cone));
	if (!cone)
		return NULL;
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
	if (vec_lenght(cone->axis) == 0) {
		free(cone);
		return (NULL);
	}
	return (cone);
}

int ft_add_cone(t_scene **scene, char **components)
{
	t_cone *cone;
	t_cone *tmp;

	cone = ft_new_cone(components);
	if (!cone)
		return 0;

	tmp = (*scene)->cone;
	if (!tmp)
		(*scene)->cone = cone;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = cone;
	}
	return 1;
}