/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:17:39 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:17:40 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vector	vector_add(t_vector v1, t_vector v2)
{
	t_vector	vec_add;

	vec_add.x = v1.x + v2.x;
	vec_add.y = v1.y + v2.y;
	vec_add.z = v1.z + v2.z;
	vec_add.w = v1.w + v2.w;
	return (vec_add);
}

t_vector	vector_copy(t_vector v)
{
	t_vector	vec_copy;

	vec_copy.x = v.x;
	vec_copy.y = v.y;
	vec_copy.z = v.z;
	vec_copy.w = v.w;
	return (vec_copy);
}

t_vector	vector_sub(t_vector v1, t_vector v2)
{
	t_vector	vec_sub;

	vec_sub.x = v1.x - v2.x;
	vec_sub.y = v1.y - v2.y;
	vec_sub.z = v1.z - v2.z;
	vec_sub.w = v1.w - v2.w;
	return (vec_sub);
}

t_vector	vector_cross(t_vector v1, t_vector v2)
{
	t_vector	cross_v;

	cross_v.x = v1.y * v2.z - v1.z * v2.y;
	cross_v.y = v1.z * v2.x - v1.x * v2.z;
	cross_v.z = v1.x * v2.y - v1.y * v2.x;
	cross_v.w = 1;
	return (cross_v);
}

t_vector	vector_normilze(t_vector vec)
{
	t_vector	new;
	float		magnitude;

	magnitude = vec_lenght(vec);
	if (magnitude == 0)
		return (ft_new_vector(0, 0, 0));
	new.x = vec.x / magnitude;
	new.y = vec.y / magnitude;
	new.z = vec.z / magnitude;
	new.w = vec.w / magnitude;
	return (new);
}
