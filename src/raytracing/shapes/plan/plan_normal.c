/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:39 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:39:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

t_vector	normalize_at_plane_pos(t_plane *plane)
{
	t_vector	world_normal;
	float		**inv_transpose;
	t_vector	normalized;
	t_vector	obj_normal;

	obj_normal = plane->plane_normal;
	inv_transpose = inverse_matrix(plane->transform, 4);
	if (!inv_transpose)
		inv_transpose = plane->transform;
	ft_transpose_matrix(&inv_transpose, 4, 4);
	world_normal = ft_multiply_matrix_vec(inv_transpose, obj_normal);
	world_normal.w = 0;
	normalized = vector_normilze(world_normal);
	if (inv_transpose != plane->transform)
		ft_free_matrix(inv_transpose, 4);
	return (normalized);
}
