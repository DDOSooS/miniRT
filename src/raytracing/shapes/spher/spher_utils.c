/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spher_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:59 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:07:07 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

t_sphere	*default_sphere(void)
{
	t_sphere	*sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (NULL);
	sphere->sphere_coordinates = ft_new_point(0, 0, 0);
	sphere->sphere_diameter = 1;
	sphere->transform = identity_matrix(4);
	sphere->material = default_material();
	sphere->next = NULL;
	return (sphere);
}
