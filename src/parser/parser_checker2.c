/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checker2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:58:33 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:20:03 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	check_xpm_file(char *file)
{
	char	*extention;

	extention = ft_strrchr(file, '.');
	if (extention && ft_strcmp(extention, ".xpm") == 0)
		return (1);
	printf("invalid xpm file\n");
	return (0);
}

int	ft_check_sphere_component(char **components)
{
	int	cout;

	cout = ft_count_components(components);
	if (cout != 4 && cout != 6 && cout != 5)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_non_negative(components[2]))
		return (0);
	if (!ft_check_colors(components[3]))
		return (0);
	if (cout == 5)
		return (check_xpm_file(components[4]));
	if (cout == 6)
	{
		if (ft_strcmp(components[4], "ch") != 0)
			return (0);
		if (!ft_check_colors(components[5]))
			return (0);
	}
	return (1);
}

int	ft_check_plane_component(char **components)
{
	if (ft_count_components(components) != 4)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_elements(components[2]) || !ft_check_norm(components[2]))
		return (0);
	if (!ft_check_colors(components[3]))
		return (0);
	return (1);
}

int	ft_check_cylinder_component(char **components)
{
	if (ft_count_components(components) != 6)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_elements(components[2]) || !ft_check_norm(components[2]))
		return (0);
	if (!ft_check_non_negative(components[3])
		|| !ft_check_non_negative(components[4]))
		return (0);
	if (!ft_check_colors(components[5]))
		return (0);
	return (1);
}

int	ft_check_cone_component(char **components)
{
	if (ft_count_components(components) != 6)
		return (0);
	if (!ft_check_elements(components[1]))
		return (0);
	if (!ft_check_elements(components[2]) || !ft_check_norm(components[2]))
		return (0);
	if (!ft_check_non_negative(components[3])
		|| !ft_check_non_negative(components[4]))
		return (0);
	if (!ft_check_colors(components[5]))
		return (0);
	return (1);
}
