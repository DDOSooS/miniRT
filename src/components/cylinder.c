/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:21 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:10:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_cylinder	*ft_new_cylinder(char **components)
{
	t_cylinder	*new;

	new = malloc(sizeof(t_cylinder));
	if (!new)
		return (NULL);
	ft_gen_elements(&new->coordinates, components[1]);
	ft_gen_elements(&new->orientation, components[2]);
	new->raduis = ft_atod(components[3]);
	new->height = ft_atod(components[4]);
	ft_gen_colors(&new->cylinder_color, components[5]);
	new->transform = identity_matrix(4);
	new->transform[0][3] = new->coordinates.x;
	new->transform[1][3] = new->coordinates.y;
	new->transform[2][3] = new->coordinates.z;
	new->material = default_material();
	new->material->color = new->cylinder_color;
	new->next = NULL;
	return (new);
}

int	ft_add_cylinder(t_scene **scene, char **components)
{
	t_cylinder	*new;
	t_cylinder	*tmp;

	new = ft_new_cylinder(components);
	tmp = (*scene)->cylinder;
	if (!tmp)
		(*scene)->cylinder = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}
