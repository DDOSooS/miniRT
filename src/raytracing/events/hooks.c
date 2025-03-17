/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 08:50:50 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 09:32:44 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../../../includes/minirt.h"

void	setup_window_hooks(t_scene *scene)
{
	mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene);
	mlx_key_hook(scene->data->win, &key_hook, scene);
	mlx_loop(scene->data->mlx);
}

int	key_hook(int keycode, t_scene *data)
{
	if (keycode == ESC_KEY)
	{
		ft_close_window(data);
		exit(0);
	}
	return (0);
}

int	ft_close(t_var *vars)
{
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
	return (0);
}
