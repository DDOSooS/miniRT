/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:31 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:10:32 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_plane	*ft_new_plane(char **components)
{
	t_plane	*plane;

	plane = malloc(sizeof(t_plane));
	if (!plane)
		return (NULL);
	ft_gen_elements(&plane->plane_cordinates, components[1]);
	ft_gen_elements(&plane->plane_normal, components[2]);
	ft_gen_colors(&plane->plane_color, components[3]);
	plane->transform = identity_matrix(4);
	plane->material = default_material();
	plane->material->color = plane->plane_color;
	plane->next = NULL;
	return (plane);
}

int	ft_add_plane(t_scene **scene, char **components)
{
	t_plane	*new;
	t_plane	*tmp;

	new = ft_new_plane(components);
	tmp = (*scene)->plane;
	if (!tmp)
		(*scene)->plane = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}
