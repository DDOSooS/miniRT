/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spher_norm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:54 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:48:20 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

t_vector	normilize_at_sphere_pos(t_sphere *sphere, t_point w_p)
{
	t_vector	vec;
	t_vector	obj_n;
	t_vector	tmp;
	float		**inv_m;

	inv_m = inverse_matrix(sphere->transform, 4);
	if (!inv_m)
		inv_m = sphere->transform;
	obj_n = vector_sub(w_p, sphere->sphere_coordinates);
	ft_transpose_matrix(&inv_m, 4, 4);
	tmp = ft_multiply_matrix_vec(inv_m, obj_n);
	tmp.w = 0;
	vec = vector_normilze(tmp);
	if (inv_m != sphere->transform)
		ft_free_matrix(inv_m, 4);
	return (vec);
}
