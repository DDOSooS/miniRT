/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:28 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/16 23:49:30 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	compare_vector(float a, float b)
{
	if (fabs(a - b) < EPSILON)
		return (1);
	return (0);
}

float	**ft_create_matrix(int n_rows, int n_cols)
{
	float	**matrix;
	int		i;
	int		j;

	matrix = (float **)malloc(n_rows * sizeof(float *));
	if (!matrix)
		return (NULL);
	i = -1;
	while (++i < n_rows)
	{
		j = -1;
		matrix[i] = (float *)malloc(n_cols * sizeof(float));
		if (!matrix[i])
		{
			while (++j < i)
				free(matrix[j]);
			free(matrix);
			return (NULL);
		}
		while (++j < n_cols)
			matrix[i][j] = 0;
	}
	return (matrix);
}

int	ft_compare_matrix(float **m1, float **m2, int n_col, int n_row)
{
	int	i;
	int	j;

	i = -1;
	while (++i < n_row)
	{
		j = -1;
		while (++j < n_col)
		{
			if (fabs(m1[i][j] - m2[i][j]) >= EPSILON)
				return (0);
		}
	}
	return (1);
}

float	**ft_multiply_matrix(float **m1, float **m2, int n_cols, int n_row)
{
	float	**result;
	int		i;
	int		j;
	int		k;

	i = -1;
	result = ft_create_matrix(n_row, n_cols);
	if (!result)
		return (NULL);
	while (++i < n_row)
	{
		j = -1;
		while (++j < n_cols)
		{
			result[i][j] = 0;
			k = -1;
			while (++k < n_cols)
				result[i][j] += m1[i][k] * m2[k][j];
		}
	}
	return (result);
}

t_vector	ft_multiply_matrix_vec(float **m, t_vector v)
{
	t_vector	result;

	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return (result);
}
