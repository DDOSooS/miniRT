/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freieer_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:21:43 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:57:16 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../includes/minirt.h"

void	ft_free(char ***words, int size)
{
	int	i;

	i = -1;
	while (++i < size)
		free((*words)[i]);
	free(*words);
	*words = NULL;
}

void	ft_free_line_components(char **components)
{
	int	i;

	i = 0;
	while (components[i])
	{
		free(components[i]);
		i++;
	}
	free(components);
}

void	ft_free_map(t_map_line **map_lines)
{
	t_map_line	*tmp;

	while (*map_lines)
	{
		tmp = (*map_lines)->next;
		ft_free_line_components((*map_lines)->line_component);
		free(*map_lines);
		(*map_lines) = tmp;
	}
	map_lines = NULL;
}

void	free_matrix(float **m)
{
	int	i;

	i = -1;
	while (++i < 4)
		free(m[i]);
	free(m);
}
