/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:47 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/16 23:49:54 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_point	ft_multiply_matrix_point(float **m, t_point v)
{
	t_vector	result;

	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return (result);
}

void	ft_scale_matrix(float ***m, float scale, int n)
{
	int	i;
	int	j;

	i = -1;
	while (++i < n)
	{
		j = -1;
		while (++j < n)
			(*m)[i][j] *= scale;
	}
}

void	ft_transpose_matrix(float ***m, int n_rows, int n_col)
{
	float	temp;
	int		i;
	int		j;

	i = -1;
	while (++i < n_rows)
	{
		j = -1;
		while (++j < n_col)
		{
			temp = (*m)[i][j];
			(*m)[i][j] = (*m)[j][i];
			(*m)[j][i] = temp;
		}
	}
}

float	**ft_translate_matrix(t_point point, int inverse)
{
	float	**translation_matrix;
	int		i;
	int		j;

	i = -1;
	translation_matrix = ft_create_matrix(4, 4);
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			if (i == j && i < 3)
				translation_matrix[i][j] = 1;
			else
				translation_matrix[i][j] = 0;
		}
	}
	translation_matrix[0][3] = inverse * point.x;
	translation_matrix[1][3] = inverse * point.y;
	translation_matrix[2][3] = inverse * point.z;
	translation_matrix[3][3] = inverse * point.w;
	return (translation_matrix);
}

void	ft_free_matrix(float **m, int n)
{
	int	i;

	i = -1;
	while (++i < n)
		free(m[i]);
	free(m);
	m = NULL;
}
