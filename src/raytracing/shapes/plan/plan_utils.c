/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plan_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:49:44 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:06:45 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minirt.h"

void	ft_add_plane_shape(t_world *world, t_plane *plane)
{
	t_plane	*tmp;

	tmp = plane;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_PLANE);
		tmp = tmp->next;
	}
}
