/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:48:45 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 01:48:48 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	ft_is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

double	get_fractional(char *str, int i)
{
	double	fractional_part;
	double	divisor;

	fractional_part = 0.0;
	divisor = 10.0;
	while (str[++i] && (str[i] >= '0' && str[i] <= '9'))
	{
		fractional_part += (str[i] - '0') / divisor;
		divisor *= 10.0;
	}
	return (fractional_part);
}

double	ft_atod(char *str)
{
	double	result;
	int		i;
	int		sign;

	result = 0.0;
	i = 0;
	sign = 1;
	while (ft_is_whitespace(str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	i--;
	while (str[++i] && (str[i] >= '0' && str[i] <= '9'))
		result = result * 10 + (str[i] - '0');
	if (str[i] == '.')
		result += get_fractional(str, i);
	return (result * sign);
}

int	is_empty_line(char *line)
{
	int	i;

	i = 0;
	if (!line || line[0] == '\n')
		return (1);
	while (line[i])
	{
		if (!ft_is_whitespace(line[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_check_file_name(char *filename)
{
	char	*extention;

	extention = ft_strrchr(filename, '.');
	if (extention && ft_strcmp(extention, ".rt") == 0)
		return (1);
	return (0);
}
