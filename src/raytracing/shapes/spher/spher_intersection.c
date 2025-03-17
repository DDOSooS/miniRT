/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spher_intersection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:50 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:47:56 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

float	calculate_discriment(t_quadratic *tmp, t_ray ray, t_sphere *sphere)
{
	t_vector	sphere_to_ray;

	sphere_to_ray = vector_sub(ray.origin, sphere->sphere_coordinates);
	tmp->a = vector_dot(ray.direction, ray.direction);
	tmp->b = 2 * vector_dot(ray.direction, sphere_to_ray);
	tmp->c = vector_dot(sphere_to_ray, sphere_to_ray) - (sphere->sphere_diameter
			/ 2.0 * sphere->sphere_diameter / 2.0);
	return (tmp->b * tmp->b - 4 * tmp->a * tmp->c);
}

void	ft_set_equation_sol(t_intersection *result, t_sphere *sphere,
		float closest_t)
{
	result->t1 = closest_t;
	result->n_sol = 2;
	result->object = sphere;
	result->type = SHAPE_SPHERE;
}

t_intersection	ft_intersect_sphere(t_ray ray, t_sphere *sphere)
{
	t_intersection	result;
	float			closest_t;
	float			discriminant;
	t_quadratic		sol;

	result = ft_init_intersection(SHAPE_SPHERE);
	discriminant = calculate_discriment(&sol, ray, sphere);
	if (discriminant >= 0)
	{
		sol.t1 = (-sol.b - sqrtf(discriminant)) / (2 * sol.a);
		sol.t2 = (-sol.b + sqrtf(discriminant)) / (2 * sol.a);
		closest_t = -1;
		if (sol.t1 > EPSILON)
			closest_t = sol.t1;
		if (sol.t2 > EPSILON && (closest_t < 0 || sol.t2 < closest_t))
			closest_t = sol.t2;
		if (closest_t > 0)
			ft_set_equation_sol(&result, sphere, closest_t);
	}
	return (result);
}
