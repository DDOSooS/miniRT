/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:24 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/16 23:49:25 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	ft_new_color(float r, float g, float b)
{
	t_color	new;

	new.r = r;
	new.g = g;
	new.b = b;
	return (new);
}

t_color	ft_add_color(t_color c1, t_color c2)
{
	t_color	new;

	new.r = c1.r + c2.r;
	new.g = c1.g + c2.g;
	new.b = c1.b + c2.b;
	return (new);
}

t_color	ft_sub_color(t_color c1, t_color c2)
{
	t_color	new;

	new.r = c1.r - c2.r;
	new.g = c1.g - c2.g;
	new.b = c1.b - c2.b;
	return (new);
}

t_color	ft_scale_color(t_color c, float scalar)
{
	t_color	new;

	new.r = c.r * scalar;
	new.g = c.g * scalar;
	new.b = c.b * scalar;
	return (new);
}

t_color	ft_multiply_color(t_color c1, t_color c2)
{
	t_color	new;

	new.r = c1.r * c2.r;
	new.g = c1.g * c2.g;
	new.b = c1.b * c2.b;
	return (new);
}
