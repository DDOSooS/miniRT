/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:18:21 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:18:31 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_vector	ft_new_vector(float x, float y, float z)
{
	t_vector	new;

	new.x = x;
	new.y = y;
	new.z = z;
	new.w = 0;
	return (new);
}

t_vector	ft_scale_vector(t_vector vector, float scale)
{
	t_vector	new;

	new.x = vector.x * scale;
	new.y = vector.y * scale;
	new.z = vector.z * scale;
	new.w = vector.w * scale;
	return (new);
}

t_vector	negate_vector(t_vector vector)
{
	t_vector	new;

	new.x = -vector.x;
	new.y = -vector.y;
	new.z = -vector.z;
	new.w = -vector.w;
	return (new);
}
