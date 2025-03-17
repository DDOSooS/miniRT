/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:02:06 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:00:13 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../includes/minirt.h"

t_shape	*ft_new_shape(void *shape_obj, int shape_type)
{
	t_shape	*new_shape;

	new_shape = malloc(sizeof(t_shape));
	if (!new_shape)
		return (NULL);
	new_shape->type = shape_type;
	new_shape->next = NULL;
	if (shape_type == SHAPE_SPHERE)
		new_shape->u_objects.sphere = (t_sphere *)shape_obj;
	else if (shape_type == SHAPE_PLANE)
		new_shape->u_objects.plane = (t_plane *)shape_obj;
	else if (shape_type == SHAPE_CYLINDER)
		new_shape->u_objects.cylinder = (t_cylinder *)shape_obj;
	else if (shape_type == SHAPE_CONE)
		new_shape->u_objects.cone = (t_cone *)shape_obj;
	else
	{
		free(new_shape);
		return (NULL);
	}
	return (new_shape);
}

t_compose	*init_composition(t_intersection inter, t_ray ray)
{
	t_compose	*comp;

	comp = malloc(sizeof(t_compose));
	comp->t = inter.t1;
	comp->obj = inter.object;
	comp->obj_type = inter.type;
	comp->camv = negate_vector(ray.direction);
	comp->point = position(ray, comp->t);
	return (comp);
}
