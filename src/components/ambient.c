/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:09:57 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:10:01 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_add_ambient(t_scene **scene, char **components)
{
	t_ambient	*tmp;

	tmp = malloc(sizeof(t_ambient));
	tmp->ambient_ration = ft_atod(components[1]);
	ft_gen_colors(&tmp->ambient_color, components[2]);
	(*scene)->ambient = tmp;
	return (1);
}
