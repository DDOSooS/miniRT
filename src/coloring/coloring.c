/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coloring.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 03:06:50 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 06:10:59 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_color	handle_standard_object(t_world *world, t_compose *comp, t_light *light,
		t_material *material)
{
	comp->shadow = is_shadowed(world, light, comp->over_point);
	return (get_lighting_color(material, light, comp,
			material->color));
}

t_color	process_light(t_world *world, t_compose *comp, t_light *light,
		t_material *material)
{
	t_sphere	*sphere;

	if (comp->obj_type == SHAPE_SPHERE)
	{
		sphere = (t_sphere *)comp->obj;
		if (sphere->has_checkered)
			return (handle_checkered_sphere(world, comp, light, material));
		else if (sphere->has_texture)
			return (handle_textured_sphere(world, comp, light, material));
	}
	return (handle_standard_object(world, comp, light, material));
}
