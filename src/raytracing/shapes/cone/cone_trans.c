/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_trans.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:14:47 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 09:17:48 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

static void	init_rotation_matric(float rotation_matrix[4][4])
{
	rotation_matrix[0][0] = 1;
	rotation_matrix[0][1] = 0;
	rotation_matrix[0][2] = 0;
	rotation_matrix[0][3] = 0;
	rotation_matrix[1][0] = 0;
	rotation_matrix[1][1] = -1;
	rotation_matrix[1][2] = 0;
	rotation_matrix[1][3] = 0;
	rotation_matrix[2][0] = 0;
	rotation_matrix[2][1] = 0;
	rotation_matrix[2][2] = -1;
	rotation_matrix[2][3] = 0;
	rotation_matrix[3][0] = 0;
	rotation_matrix[3][1] = 0;
	rotation_matrix[3][2] = 0;
	rotation_matrix[3][3] = 1;
}

static void	handle_special_case(float **rotation)
{
	float	temp_matrix[4][4];

	init_rotation_matric(temp_matrix);
	copy_matrix_cone(rotation, temp_matrix);
}

static void	handle_normal_case(float **rotation, t_vector axis)
{
	float				temp_matrix[4][4];
	t_transform_vars	vars;

	vars.axis = vector_normilze(axis);
	vars.up = (t_vector){0, 1, 0, 0};
	vars.dot = vector_dot(vars.axis, vars.up);
	vars.angle = acos(vars.dot);
	vars.cross = vector_cross(vars.up, vars.axis);
	if (fabs(vars.angle) > 0.0001)
	{
		create_rotation_matrix(vars.cross, vars.angle, temp_matrix);
		copy_matrix_cone(rotation, temp_matrix);
	}
}

static float	**create_rotation_matrix_from_vector_cone(t_vector axis)
{
	float		**rotation;
	t_vector	normalized_axis;

	rotation = create_identity_matrix(4);
	normalized_axis = vector_normilze(axis);
	if (fabs(normalized_axis.x) < 0.0001 && fabs(normalized_axis.y + 1) < 0.0001
		&& fabs(normalized_axis.z) < 0.0001)
		handle_special_case(rotation);
	else
		handle_normal_case(rotation, normalized_axis);
	return (rotation);
}

float	**create_cone_transform(t_cone *cone)
{
	float	**rotation;
	float	**translation;
	float	**combined;

	rotation = create_rotation_matrix_from_vector_cone(cone->axis);
	translation = create_translation_matrix(cone);
	combined = matrix_multiply(rotation, translation);
	free_matrix(rotation);
	free_matrix(translation);
	return (combined);
}
