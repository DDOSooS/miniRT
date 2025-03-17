/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkering.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 03:28:23 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 03:28:27 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	get_checkered_color(t_sphere *sphere, float u, float v)
{
	int	scale;
	int	u_scaled;
	int	v_scaled;

	scale = 40;
	u_scaled = (int)(u * scale);
	v_scaled = (int)(v * scale);
	if ((u_scaled + v_scaled) % 2 == 0)
		return (sphere->sphere_color);
	return (sphere->checkered_color);
}

t_color	handle_checkered_sphere(t_world *world, t_compose *comp, t_light *light,
		t_material *material)
{
	float	u;
	float	v;
	int		shadowed;
	t_color	base_color;

	u = 0;
	v = 0;
	get_spherical_coordinates(comp->over_point, (t_sphere *)comp->obj, &u, &v);
	base_color = get_checkered_color((t_sphere *)comp->obj, u, v);
	shadowed = is_shadowed(world, light, comp->over_point);
	return (get_lighting_color(material, light, comp, shadowed, base_color));
}
