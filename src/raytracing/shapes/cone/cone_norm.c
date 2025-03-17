/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_norm.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:30:27 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 09:37:02 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

static float	**matrix_inverse(void)
{
	return (create_identity_matrix(4));
}

t_vector	compute_local_normal(t_vector local_point)
{
	t_vector	apex_to_point;
	float		height_component;
	t_vector	normal;

	apex_to_point = vector_sub(local_point, (t_vector){0, 0, 0, 0});
	height_component = vector_dot(apex_to_point, (t_vector){0, 1, 0, 0});
	normal = vector_sub(apex_to_point, vector_mult_scalar((t_vector){0, 1, 0,
				0}, height_component));
	return (normal);
}

t_vector	normalize_at_cone_pos(t_cone *cone, t_vector point)
{
	float		**transform;
	float		**inverse_transform;
	t_vector	local_point;
	t_vector	normal;
	t_vector	world_normal;

	transform = create_cone_transform(cone);
	local_point = matrix_multiply_vector(transform, point);
	normal = compute_local_normal(local_point);
	inverse_transform = matrix_inverse();
	world_normal = matrix_multiply_vector(inverse_transform, normal);
	free_matrix(transform);
	free_matrix(inverse_transform);
	return (vector_normilze(world_normal));
}
