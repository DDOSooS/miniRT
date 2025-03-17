/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 11:46:07 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 10:12:18 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

char	*int_to_str(int num)
{
	char	*str;

	int (len), (temp);
	len = 0;
	temp = num;
	if (temp == 0)
		len = 1;
	else
	{
		while (temp != 0)
		{
			temp /= 10;
			len++;
		}
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}

void	ft_strcpy(char *dest, const char *src)
{
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

void	ft_strcat(char *dest, const char *src)
{
	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = '\0';
}

void	display_progress(t_scene *scene, t_scamera *cam, int current_pixel,
		int total_pixels)
{
	int		progress;
	char	*progress_str;
	char	*progress_num;

	progress = (int)((float)current_pixel / total_pixels * 100);
	progress_str = malloc(20);
	if (!progress_str)
		return ;
	progress_num = int_to_str(progress);
	if (!progress_num)
	{
		free(progress_str);
		return ;
	}
	ft_strcpy(progress_str, "Loading... ");
	ft_strcat(progress_str, progress_num);
	ft_strcat(progress_str, "%");
	mlx_clear_window(scene->data->mlx, scene->data->win);
	mlx_string_put(scene->data->mlx, scene->data->win, cam->w_size / 2 - 50,
		cam->h_size / 2, 0xFFFFFF, progress_str);
	free(progress_str);
	free(progress_num);
}
