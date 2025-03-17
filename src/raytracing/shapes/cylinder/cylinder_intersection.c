/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersection.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:48:42 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:05:50 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

void	calculate_cylinder_discriment(t_cylinder_eq *tmp, t_cylinder *cylinder,
		t_ray ray, float **inverse)
{
	tmp->counter = 0;
	tmp->is_inside = 0;
	tmp->half_height = cylinder->height / 2.0f;
	tmp->tr_ray = create_ray(ft_multiply_matrix_vec(inverse, ray.origin),
			ft_multiply_matrix_vec(inverse, ray.direction));
	tmp->a = tmp->tr_ray.direction.x * tmp->tr_ray.direction.x
		+ tmp->tr_ray.direction.z * tmp->tr_ray.direction.z;
	tmp->b = 2.0f * (tmp->tr_ray.origin.x * tmp->tr_ray.direction.x
			+ tmp->tr_ray.origin.z * tmp->tr_ray.direction.z);
	tmp->c = tmp->tr_ray.origin.x * tmp->tr_ray.origin.x + tmp->tr_ray.origin.z
		* tmp->tr_ray.origin.z - cylinder->raduis / 2.f * cylinder->raduis
		/ 2.f;
	tmp->discriminant = tmp->b * tmp->b - 4.0f * tmp->a * tmp->c;
}

void	descriminant_intersction(t_cylinder_eq *tmp)
{
	float	sqrt_disc;
	float	t1;
	float	t2;
	float	y1;
	float	y2;

	sqrt_disc = sqrtf(tmp->discriminant);
	t1 = (-tmp->b - sqrt_disc) / (2.0f * tmp->a);
	t2 = (-tmp->b + sqrt_disc) / (2.0f * tmp->a);
	y1 = tmp->tr_ray.origin.y + t1 * tmp->tr_ray.direction.y;
	y2 = tmp->tr_ray.origin.y + t2 * tmp->tr_ray.direction.y;
	if (y1 >= -tmp->half_height && y1 <= tmp->half_height)
		tmp->sol[tmp->counter++] = t1;
	if (y2 >= -tmp->half_height && y2 <= tmp->half_height)
		tmp->sol[tmp->counter++] = t2;
}

void	set_result_equation(t_intersection *result, t_cylinder_eq *tmp,
		t_ray ray, t_cylinder *cylinder)
{
	{
		tmp->min_t = get_min_sol(tmp->sol, tmp->counter);
		tmp->is_inside = is_point_inside_cylinder(position(ray, tmp->min_t),
				cylinder);
		if (tmp->min_t > EPSILON || tmp->is_inside)
		{
			result->n_sol = 1;
			result->object = cylinder;
			result->t1 = tmp->min_t;
		}
	}
}

t_intersection	ft_init_intersection(int type)
{
	t_intersection	new;

	new.t1 = -1;
	new.n_sol = -1;
	new.object = NULL;
	new.type = type;
	return (new);
}

t_intersection	ft_intersect_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_cylinder_eq	tmp;
	t_intersection	result;
	float			**inverse;
	t_intersection	captop;
	t_intersection	capbottom;

	result = ft_init_intersection(SHAPE_CYLINDER);
	inverse = get_combined_inv(cylinder);
	calculate_cylinder_discriment(&tmp, cylinder, ray, inverse);
	if (tmp.discriminant >= 0)
		descriminant_intersction(&tmp);
	captop = check_intersection_caps(tmp.tr_ray, cylinder, tmp.half_height);
	if (captop.n_sol > 0)
		tmp.sol[tmp.counter++] = captop.t1;
	capbottom = check_intersection_caps(tmp.tr_ray, cylinder, -tmp.half_height);
	if (capbottom.n_sol > 0)
		tmp.sol[tmp.counter++] = capbottom.t1;
	if (tmp.counter > 0)
		set_result_equation(&result, &tmp, ray, cylinder);
	ft_free_matrix(inverse, 4);
	return (result);
}
