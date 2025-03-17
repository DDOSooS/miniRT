/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils5.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:17:18 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:17:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

float	**shearing_matrix(int *coord)
{
	float	**shearing_matrix;
	int		i;
	int		j;

	i = -1;
	shearing_matrix = ft_create_matrix(4, 4);
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			shearing_matrix[i][j] = 0;
			if (i == j)
				shearing_matrix[i][j] = 1;
		}
	}
	shearing_matrix[0][1] = coord[0];
	shearing_matrix[0][2] = coord[1];
	shearing_matrix[1][0] = coord[2];
	shearing_matrix[1][2] = coord[3];
	shearing_matrix[2][0] = coord[4];
	shearing_matrix[2][1] = coord[5];
	return (shearing_matrix);
}

void	get_rotate_y_helper(float rad, int i, int j, float **m)
{
	if ((i == 0 && j == 0) || (i == 2 && j == 2))
		m[i][j] = cos(rad);
	else if (i == 2 && j == 0)
		m[i][j] = -sin(rad);
	else if (i == 0 && j == 2)
		m[i][j] = sin(rad);
	else
		m[i][j] = 0;
}

float	**rotate_y(float rad)
{
	float	**m;
	int		i;
	int		j;

	m = ft_create_matrix(4, 4);
	if (!m)
		return (NULL);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			get_rotate_y_helper(rad, i, j, m);
	}
	m[1][1] = 1;
	m[3][3] = 1;
	return (m);
}

void	get_rotate_z_helper(float rad, int i, int j, float **m)
{
	if ((i == 1 && j == 1) || (i == 0 && j == 0))
		m[i][j] = cos(rad);
	else if (i == 0 && j == 1)
		m[i][j] = -sin(rad);
	else if (i == 1 && j == 1)
		m[i][j] = sin(rad);
	else
		m[i][j] = 0;
}

float	**rotate_z(float rad)
{
	float	**m;
	int		i;
	int		j;

	m = ft_create_matrix(4, 4);
	if (!m)
		return (NULL);
	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
			get_rotate_y_helper(rad, i, j, m);
	}
	m[2][2] = 1;
	m[3][3] = 1;
	return (m);
}
