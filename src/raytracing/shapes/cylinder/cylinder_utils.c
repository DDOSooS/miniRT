/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:24 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:18:23 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

float	**get_combined_inv(t_cylinder *cy)
{
	float	**rotation;
	float	**combined;
	float	**inv;

	rotation = create_rotation_matrix_from_vector(cy->orientation);
	combined = ft_multiply_matrix(cy->transform, rotation, 4, 4);
	inv = inverse_matrix(combined, 4);
	if (!inv)
		inv = copy_matrix(cy->transform);
	ft_free_matrix(rotation, 4);
	ft_free_matrix(combined, 4);
	return (inv);
}

void	ft_add_cylinder_shape(t_world *world, t_cylinder *cylinder)
{
	t_cylinder	*tmp;

	tmp = cylinder;
	while (tmp)
	{
		if (tmp->orientation.x != 0 || tmp->orientation.y != 0
			|| tmp->orientation.z != 0)
			ft_add_shape(&world, tmp, SHAPE_CYLINDER);
		tmp = tmp->next;
	}
}
