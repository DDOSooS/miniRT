/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:21 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/16 23:49:23 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	else
		return (value);
}

t_color	clamp_color(t_color color)
{
	color.r = clamp(color.r, 0.0f, 1.0f);
	color.g = clamp(color.g, 0.0f, 1.0f);
	color.b = clamp(color.b, 0.0f, 1.0f);
	return (color);
}

t_color	ft_multiply_color_scalar(t_color color, float scalar)
{
	t_color	new_color;

	new_color.r = color.r * scalar;
	new_color.g = color.g * scalar;
	new_color.b = color.b * scalar;
	return (new_color);
}
