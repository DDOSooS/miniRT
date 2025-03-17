/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:23:41 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:53:36 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

float	**create_matrix(int rows, int cols)
{
	float	**m;
	int		i;

	i = -1;
	m = malloc(sizeof(float *) * rows);
	while (++i < rows)
		m[i] = malloc(sizeof(float) * cols);
	return (m);
}

float	**create_identity_matrix(int size)
{
	float	**m;
	int		i;
	int		j;

	i = -1;
	m = create_matrix(size, size);
	while (++i < size)
	{
		j = -1;
		while (++j < size)
		{
			if (i == j)
				m[i][j] = 1;
			else
				m[i][j] = 0;
		}
	}
	return (m);
}

float	**matrix_multiply(float **a, float **b)
{
	float	**result;
	int		i;
	int		j;
	int		k;

	i = -1;
	result = create_matrix(4, 4);
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			result[i][j] = 0;
			k = -1;
			while (++k < 4)
				result[i][j] += a[i][k] * b[k][j];
		}
	}
	return (result);
}

t_point	point_add(t_point point, t_vector p2)
{
	t_point	new_point;

	new_point.x = point.x + p2.x;
	new_point.y = point.y + p2.y;
	new_point.z = point.z + p2.z;
	new_point.w = 1.0;
	return (new_point);
}

t_point	position(t_ray ray, float distance)
{
	t_point	new;

	new.x = ray.origin.x + ray.direction.x * distance;
	new.y = ray.origin.y + ray.direction.y * distance;
	new.z = ray.origin.z + ray.direction.z * distance;
	new.w = 1.0;
	return (new);
}
