/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 09:25:34 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 09:17:32 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

static void	calculate_cone_coefficients(t_ray local_ray, t_cone *cone,
		t_quadratic *coeff)
{
	t_vector	oc;
	float		k;
	float		k2;

	oc = vector_sub(local_ray.origin, (t_vector){0, 0, 0, 0});
	k = cone->radius / cone->height;
	k2 = k * k;
	coeff->a = local_ray.direction.x * local_ray.direction.x
		+ local_ray.direction.z * local_ray.direction.z - k2
		* local_ray.direction.y * local_ray.direction.y;
	coeff->b = 2 * (local_ray.direction.x * oc.x + local_ray.direction.z * oc.z
			- k2 * local_ray.direction.y * oc.y);
	coeff->c = oc.x * oc.x + oc.z * oc.z - k2 * oc.y * oc.y;
}

static int	is_point_in_cone_bounds(float y, float height)
{
	return (y >= 0 && y <= height);
}

/* main function */
static void	calculate_intersection_points(t_ray local_ray, t_cone *cone,
		t_intdata t_values, t_intdata *valid_t)
{
	float	y1;
	float	y2;

	y1 = local_ray.origin.y + t_values.t1 * local_ray.direction.y;
	y2 = local_ray.origin.y + t_values.t2 * local_ray.direction.y;
	if (is_point_in_cone_bounds(y1, cone->height))
		valid_t->t1 = t_values.t1;
	else
		valid_t->t1 = -1;
	if (is_point_in_cone_bounds(y2, cone->height))
		valid_t->t2 = t_values.t2;
	else
		valid_t->t2 = -1;
}

static void	init_intersection(t_intersection *inter)
{
	inter->n_sol = 0;
	inter->t1 = 0;
	inter->object = NULL;
	inter->type = 0;
}

/* main function */
t_intersection	ft_intersect_cone(t_ray ray, t_cone *cone)
{
	t_intersection	inter;
	t_quadratic		coeff;
	t_intdata		ts;
	t_ray			local_ray;
	float			discriminant;

	init_intersection(&inter);
	local_ray = get_local_ray(ray, cone);
	calculate_cone_coefficients(local_ray, cone, &coeff);
	discriminant = coeff.b * coeff.b - 4 * coeff.a * coeff.c;
	if (discriminant < 0)
		return (inter);
	discriminant = sqrt(discriminant);
	ts.t1 = (-coeff.b - discriminant) / (2 * coeff.a);
	ts.t2 = (-coeff.b + discriminant) / (2 * coeff.a);
	calculate_intersection_points(local_ray, cone, ts, &ts);
	if (ts.t1 >= 0 || ts.t2 >= 0)
	{
		inter.n_sol = 2;
		inter.t1 = ts.t1;
		inter.object = cone;
		inter.type = SHAPE_CONE;
	}
	return (inter);
}
