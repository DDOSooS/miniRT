/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 21:03:01 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 09:17:43 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

void	copy_matrix_cone(float **dst, float temp_matrix[4][4])
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			dst[i][j] = temp_matrix[i][j];
			j++;
		}
		i++;
	}
}

float	**create_translation_matrix(t_cone *cone)
{
	float	**transform;

	transform = create_identity_matrix(4);
	transform[0][3] = -cone->apex.x;
	transform[1][3] = -cone->apex.y;
	transform[2][3] = -cone->apex.z;
	return (transform);
}

static t_ray	transform_ray(t_ray ray, float **transform)
{
	t_ray	transformed_ray;

	ray.origin.w = 1;
	transformed_ray.origin = matrix_multiply_vector(transform, ray.origin);
	ray.direction.w = 0;
	transformed_ray.direction = matrix_multiply_vector(transform,
			ray.direction);
	transformed_ray.direction.w = 0;
	return (transformed_ray);
}

t_ray	get_local_ray(t_ray ray, t_cone *cone)
{
	float	**transform;
	t_ray	local_ray;

	transform = create_cone_transform(cone);
	local_ray = transform_ray(ray, transform);
	free_matrix(transform);
	return (local_ray);
}
