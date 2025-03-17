/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:17:29 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:17:29 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_point	ft_new_point(float x, float y, float z)
{
	t_point	new;

	new.x = x;
	new.y = y;
	new.z = z;
	new.w = 1;
	return (new);
}

t_point	ft_scale_point(t_point vector, float scale)
{
	t_vector	new;

	new.x = vector.x * scale;
	new.y = vector.y * scale;
	new.z = vector.z * scale;
	new.w = vector.w * scale;
	return (new);
}
