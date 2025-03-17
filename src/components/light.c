/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:27 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:13:08 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_add_light(t_scene **scene, char **components)
{
	t_light	*light;
	t_light	*tmp;

	light = malloc(sizeof(t_light));
	ft_gen_elements(&light->coordinate, components[1]);
	light->ration = ft_atod(components[2]);
	ft_gen_colors(&light->color, components[3]);
	light->coordinate.w = 1;
	light->next = NULL;
	if (!(*scene)->light)
		(*scene)->light = light;
	else
	{
		tmp = (*scene)->light;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = light;
	}
	return (1);
}
