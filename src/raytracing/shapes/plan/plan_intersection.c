/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan_intersection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:34 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:18:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

t_intersection	ft_intersect_plane(t_ray ray, t_plane *plane)
{
	t_intersection	result;
	float			denom;
	t_vector		origin_to_plane;
	float			t;

	result = ft_init_intersection(SHAPE_PLANE);
	denom = vector_dot(ray.direction, plane->plane_normal);
	if (fabs(denom) < EPSILON)
		return (result);
	origin_to_plane = vector_sub(ray.origin, plane->plane_cordinates);
	t = -(vector_dot(origin_to_plane, plane->plane_normal)) / denom;
	if (t < EPSILON)
		return (result);
	result.n_sol = 1;
	result.t1 = t;
	result.object = plane;
	result.type = SHAPE_PLANE;
	return (result);
}
