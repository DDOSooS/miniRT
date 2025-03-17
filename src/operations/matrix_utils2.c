/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 23:49:36 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 00:29:26 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_point	ft_transform(t_point p1, t_point p2, int inverse)
{
	t_point	transformed_point;
	float	**matrix;

	matrix = ft_translate_matrix(p1, inverse);
	transformed_point = ft_multiply_matrix_vec(matrix, p2);
	ft_free_matrix(matrix, 4);
	return (transformed_point);
}
