/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 00:09:14 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 01:20:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

float	determinant(float **m, int n)
{
	float	det;
	int		sign;
	float	**minor;
	int		i;

	if (n == 2)
		return (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	det = 0;
	sign = 1;
	i = -1;
	while (++i < n)
	{
		minor = get_minor(m, 0, i, n);
		if (!minor)
			return (0);
		det += sign * m[0][i] * determinant(minor, n - 1);
		sign = -sign;
		ft_free_matrix(minor, n - 1);
	}
	return (det);
}

int	inverse_matrix_helper(float **inverse, float **m, int n, int i)
{
	int		j;
	float	**minor;

	j = 0;
	while (++j <= n)
	{
		minor = get_minor(m, i - 1, j - 1, n);
		if (!minor)
			return (0);
		inverse[j - 1][i - 1] = pow(-1, i + j) * determinant(minor, n - 1);
		ft_free_matrix(minor, n - 1);
	}
	return (1);
}

float	**inverse_matrix(float **m, int n)
{
	float	**inverse;
	float	det;
	int		i;

	det = determinant(m, n);
	if (det == 0)
		return (NULL);
	inverse = ft_create_matrix(n, n);
	i = 0;
	while (++i <= n)
	{
		if (!inverse_matrix_helper(inverse, m, n, i))
			return (NULL);
	}
	ft_transpose_matrix(&inverse, n, n);
	ft_scale_matrix(&inverse, (float)(1.0f / det), n);
	return (inverse);
}

float	**identity_matrix(int n)
{
	float	**identity;
	int		i;
	int		j;

	identity = ft_create_matrix(n, n);
	if (!identity)
		return (NULL);
	i = -1;
	while (++i < n)
	{
		j = -1;
		while (++j < n)
		{
			if (i == j)
				identity[i][j] = 1;
			else
				identity[i][j] = 0;
		}
	}
	return (identity);
}
