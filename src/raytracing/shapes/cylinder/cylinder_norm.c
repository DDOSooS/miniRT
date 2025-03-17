/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_norm.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:48:49 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:01:57 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

t_vector	normalize_at_cylinder_pos(t_cylinder *cylinder, t_point world_p)
{
	float		**inv;
	t_vector	world_normal;
	t_point		local_p;
	t_vector	local_normal;

	inv = get_combined_inv(cylinder);
	local_p = ft_multiply_matrix_point(inv, world_p);
	if (fabs(local_p.y - cylinder->height / 2) < EPSILON)
		local_normal = ft_new_vector(0, 1, 0);
	else if (fabs(local_p.y + cylinder->height / 2) < EPSILON)
		local_normal = ft_new_vector(0, -1, 0);
	else
		local_normal = ft_new_vector(local_p.x, 0, local_p.z);
	ft_transpose_matrix(&inv, 4, 4);
	world_normal = ft_multiply_matrix_vec(inv, local_normal);
	ft_free_matrix(inv, 4);
	return (vector_normilze(world_normal));
}
