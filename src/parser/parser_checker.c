/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:58:09 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:21:09 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_check_range(char *component)
{
	double	range;

	range = ft_atod(component);
	if (range < 0 || range > 1)
		return (0);
	return (1);
}

int	ft_check_colors(char *component)
{
	char	**colors;
	int		i;
	double	color;

	colors = ftt_split(component, ",");
	if (!colors)
		return (0);
	if (ft_count_components(colors) != 3)
		return (ft_free_line_components(colors), 0);
	i = -1;
	while (colors[++i])
	{
		if (!ft_is_degit(colors[i]))
			return (ft_free_line_components(colors), 0);
		color = ft_atod(colors[i]);
		if (color < 0 || color > 255)
			return (ft_free_line_components(colors), 0);
	}
	ft_free_line_components(colors);
	return (1);
}

int	ft_check_elements(char *component)
{
	char	**coordinates;
	int		i;

	i = -1;
	coordinates = ftt_split(component, ",");
	if (ft_count_components(coordinates) != 3
		|| !ft_check_empty_element(component))
		return (ft_free_line_components(coordinates), 0);
	while (coordinates[++i])
	{
		if (!ft_is_degit(coordinates[i]))
			return (ft_free_line_components(coordinates), 0);
	}
	return (ft_free_line_components(coordinates), 1);
}

int	ft_check_non_negative(char *component)
{
	double	value;

	value = ft_atod(component);
	if (value < 0)
		return (0);
	return (1);
}

int	ft_check_fov(char *component)
{
	double	fov;

	fov = ft_atod(component);
	if (fov < 0 || fov > 180)
		return (0);
	return (1);
}
