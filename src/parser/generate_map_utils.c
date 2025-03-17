/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:49:26 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:20:52 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_is_degit(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
		i++;
	if (str[i] == '.')
	{
		i++;
		while (str[i] && (str[i] >= '0' && str[i] <= '9'))
			i++;
		if (str[i])
			return (0);
	}
	else if (str[i])
		return (0);
	return (1);
}

int	ft_count_components(char **components)
{
	int	i;

	i = 0;
	while (components[i])
		i++;
	return (i);
}

int	ft_gen_colors(t_color *color, char *components)
{
	char	**tmp_colors;

	tmp_colors = ftt_split(components, ",");
	if (!tmp_colors)
		return (0);
	(color)->r = clamp(ft_atod(tmp_colors[0]) / 255.0f, 0, 1);
	(color)->g = clamp(ft_atod(tmp_colors[1]) / 255.0f, 0, 1);
	(color)->b = clamp(ft_atod(tmp_colors[2]) / 255.0f, 0, 1);
	ft_free_line_components(tmp_colors);
	return (1);
}
