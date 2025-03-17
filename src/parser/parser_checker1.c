/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:58:23 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:21:03 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_check_norm_range(char *comp)
{
	double	tmp;

	tmp = ft_atod(comp);
	if (tmp < -1 || tmp > 1)
		return (0);
	return (1);
}

int	ft_check_norm(char *component)
{
	char	**norm;
	int		i;

	i = -1;
	norm = ftt_split(component, ",");
	while (++i < 3)
	{
		if (!ft_check_norm_range(norm[i]))
		{
			ft_free_line_components(norm);
			return (0);
		}
	}
	ft_free_line_components(norm);
	return (1);
}

int	ft_check_ambient_component(char **components, int *counter)
{
	if (ft_count_components(components) != 3)
		return (0);
	if (!ft_check_range(components[1]))
		return (0);
	if (!ft_check_colors(components[2]))
		return (0);
	(counter)[0] += 1;
	return (1);
}

int	ft_check_camera_component(char **components, int *counter)
{
	if (ft_count_components(components) != 4)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_elements(components[2]) || !ft_check_norm(components[2]))
		return (0);
	if (!ft_check_fov(components[3]))
		return (0);
	(counter)[1]++;
	return (1);
}

int	ft_check_light_component(char **components, int *counter)
{
	if (ft_count_components(components) != 4)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_range(components[2]))
		return (0);
	if (!ft_check_colors(components[3]))
		return (0);
	(counter)[2]++;
	return (1);
}
