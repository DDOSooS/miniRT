/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:17:53 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:18:07 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

inline float	vector_dot(t_vector v1, t_vector v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline double	vec_lenght(t_vector v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

t_vector	vector_multiply_scalar(t_vector v, float scalar)
{
	t_vector	new;

	new.x = v.x * scalar;
	new.y = v.y * scalar;
	new.z = v.z * scalar;
	new.w = v.w * scalar;
	return (new);
}

// Phong reflection model
t_vector	reflect_vector(t_vector in, t_vector normal)
{
	float		dot;
	t_vector	scaled_normal;
	t_vector	result;

	dot = vector_dot(in, normal);
	scaled_normal = vector_multiply_scalar(normal, 2.0f * dot);
	result = vector_sub(in, scaled_normal);
	return (result);
}
