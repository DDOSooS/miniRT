/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_rotation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:15 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:04:51 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

/*
	X-axis corresponds to left/right
	Y-axis corresponds to up/down
	Z-axis corresponds to forward/backward (with negative Z being forward)
*/

// camera transform matrix that transform from 
//world coordinates to camera system Co
// y rotation than x rotation~~~~~~

float	**create_rotation_matrix_from_vector(t_vector orientation)
{
	t_vector	new_y;
	t_vector	tmp_axis;
	float		**matrix;
	t_vector	new_x;
	t_vector	new_z;

	new_y = vector_normilze(orientation);
	tmp_axis = ft_new_vector(0, 0, 1);
	if (fabs(new_y.y) < 0.9999f)
		tmp_axis = ft_new_vector(0, 1, 0);
	new_x = vector_normilze(vector_cross(tmp_axis, new_y));
	new_z = vector_normilze(vector_cross(new_y, new_x));
	matrix = identity_matrix(4);
	matrix[0][0] = new_x.x;
	matrix[0][1] = new_y.x;
	matrix[0][2] = new_z.x;
	matrix[1][0] = new_x.y;
	matrix[1][1] = new_y.y;
	matrix[1][2] = new_z.y;
	matrix[2][0] = new_x.z;
	matrix[2][1] = new_y.z;
	matrix[2][2] = new_z.z;
	return (matrix);
}
