/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils6.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:09:12 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:55:39 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vector	vector_mult_scalar(t_vector v, float scalar)
{
	t_vector	result;

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	result.w = v.w * scalar;
	return (result);
}

float	**copy_matrix(float **m)
{
	float	**res;
	int		i;
	int		j;

	res = ft_create_matrix(4, 4);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			res[i][j] = m[i][j];
	}
	return (res);
}

t_vector	matrix_multiply_vector(float **m, t_vector v)
{
	t_vector	result;

	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return (result);
}

float	**rotation_matrix(t_vector axis, float angle)
{
	float	**m;
	float	c;
	float	s;
	float	t;

	m = identity_matrix(4);
	c = cos(angle);
	s = sin(angle);
	t = 1 - c;
	axis = vector_normilze(axis);
	m[0][0] = c + axis.x * axis.x * t;
	m[0][1] = axis.x * axis.y * t - axis.z * s;
	m[0][2] = axis.x * axis.z * t + axis.y * s;
	m[1][0] = axis.y * axis.x * t + axis.z * s;
	m[1][1] = c + axis.y * axis.y * t;
	m[1][2] = axis.y * axis.z * t - axis.x * s;
	m[2][0] = axis.z * axis.x * t - axis.y * s;
	m[2][1] = axis.z * axis.y * t + axis.x * s;
	m[2][2] = c + axis.z * axis.z * t;
	m[3][3] = 1;
	return (m);
}

float	**translation_matrix(float x, float y, float z)
{
	float	**m;

	m = create_matrix(4, 4);
	m[0][0] = 1;
	m[0][1] = 0;
	m[0][2] = 0;
	m[0][3] = x;
	m[1][0] = 0;
	m[1][1] = 1;
	m[1][2] = 0;
	m[1][3] = y;
	m[2][0] = 0;
	m[2][1] = 0;
	m[2][2] = 1;
	m[2][3] = z;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;
	return (m);
}
