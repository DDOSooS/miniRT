/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection_utils.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:05 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:24:13 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

int	check_cylinder_caps(t_ray *ray, float t, float radius)
{
	float	x;
	float	z;

	x = ray->origin.x + t * ray->direction.x;
	z = ray->origin.z + t * ray->direction.z;
	return (x * x + z * z <= radius);
}

t_intersection	check_intersection_caps(t_ray ray, t_cylinder *cylinder,
		float cap)
{
	t_intersection	result;
	float			xs;
	float			x;
	float			z;
	float			radius_squared;

	result = ft_init_intersection(SHAPE_CYLINDER);
	if (fabs(ray.direction.y) < EPSILON)
		return (result);
	xs = (cap - ray.origin.y) / ray.direction.y;
	if (xs < EPSILON)
		return (result);
	x = ray.origin.x + xs * ray.direction.x;
	z = ray.origin.z + xs * ray.direction.z;
	radius_squared = (cylinder->raduis + EPSILON) * (cylinder->raduis
			+ EPSILON);
	if ((x * x + z * z) <= radius_squared)
	{
		result.t1 = xs;
		result.n_sol = 1;
		result.object = cylinder;
	}
	return (result);
}

float	get_min_sol(float *arr, int len)
{
	int		i;
	float	min;

	i = 1;
	min = arr[0];
	while (i < len)
	{
		if (min > arr[i])
			min = arr[i];
		i++;
	}
	return (min);
}

int	is_point_inside_cylinder(t_point point, t_cylinder *cylinder)
{
	float		**inverse;
	t_vector	tr_p;
	float		half_height;
	float		radial_dist;

	inverse = get_combined_inv(cylinder);
	tr_p = ft_multiply_matrix_vec(inverse, point);
	half_height = cylinder->height / 2.0f;
	if (inverse != cylinder->transform)
		ft_free_matrix(inverse, 4);
	if (tr_p.y > half_height || tr_p.y < -half_height)
		return (0);
	radial_dist = sqrtf(tr_p.x * tr_p.x + tr_p.z * tr_p.z);
	return (radial_dist < cylinder->raduis);
}
