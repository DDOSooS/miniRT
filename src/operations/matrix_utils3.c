/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:56 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:52:04 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	get_minor_helper(float **minor, float **m, t_minor tmp)
{
	int	j;
	int	mj;

	j = -1;
	mj = 0;
	while (++j < tmp.n)
	{
		if (j == tmp.col)
			continue ;
		minor[tmp.mi][mj] = m[tmp.i][j];
		mj++;
	}
}

float	**get_minor(float **m, int row, int col, int n)
{
	int		i;
	int		mi;
	float	**minor;
	t_minor	tmp;

	minor = ft_create_matrix(n - 1, n - 1);
	if (!minor)
		return (NULL);
	mi = 0;
	tmp.n = n;
	i = -1;
	while (++i < n)
	{
		if (i == row)
			continue ;
		tmp.col = col;
		tmp.mi = mi;
		tmp.i = i;
		get_minor_helper(minor, m, tmp);
		mi++;
	}
	return (minor);
}

void	get_rotate_x_helper(float rad, int i, int j, float **m)
{
	if ((i == 1 && j == 1) || (i == 2 && j == 2))
		m[i][j] = cos(rad);
	else if (i == 1 && j == 2)
		m[i][j] = -sin(rad);
	else if (i == 2 && j == 1)
		m[i][j] = sin(rad);
	else
		m[i][j] = 0;
}

float	**rotate_x(float rad)
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
			get_rotate_x_helper(rad, i, j, m);
	}
	m[0][0] = 1;
	m[3][3] = 1;
	return (m);
}
