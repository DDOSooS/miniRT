/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkartit <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:34:45 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 02:34:47 by mkartit          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

int	open_texture_file(char *texture_name)
{
	int	fd;

	fd = open(texture_name, O_RDONLY);
	if (fd < 0)
		perror("Error: Failed to open texture file");
	else
		close(fd);
	return (fd);
}

t_texture	*allocate_texture(void)
{
	t_texture	*texture;

	texture = malloc(sizeof(t_texture));
	if (!texture)
		perror("Error: Failed to allocate memory for texture");
	return (texture);
}

int	load_texture_image(t_texture *texture, char *texture_name, t_scene **scene)
{
	texture->img_ptr = mlx_xpm_file_to_image((*scene)->data->mlx, texture_name,
			&texture->width, &texture->height);
	if (!texture->img_ptr)
	{
		perror("Error: Failed to load XPM image");
		free(texture);
		return (0);
	}
	texture->img_data = mlx_get_data_addr(texture->img_ptr, &texture->bpp,
			&texture->size_line, &texture->endian);
	if (!texture->img_data)
	{
		perror("Error: Failed to get image data");
		mlx_destroy_image(NULL, texture->img_ptr);
		free(texture);
		return (0);
	}
	return (1);
}

int	get_texture(t_sphere *sphere, char *texture_name, t_scene **scene)
{
	if (open_texture_file(texture_name) < 0)
		return (0);
	sphere->texture = allocate_texture();
	if (!sphere->texture)
		return (0);
	if (!load_texture_image(sphere->texture, texture_name, scene))
	{
		sphere->texture = NULL;
		return (0);
	}
	return (1);
}

int	handle_sphere_texture(t_sphere *sphere, char **components, int comp_count,
		t_scene **scene)
{
	if (comp_count == 6)
	{
		sphere->has_checkered = 1;
		ft_gen_colors(&sphere->checkered_color, components[5]);
	}
	else if (comp_count == 5)
	{
		sphere->has_texture = 1;
		if (!get_texture(sphere, components[4], scene))
		{
			ft_free_matrix(sphere->transform, 4);
			free(sphere->material);
			free(sphere);
			return (0);
		}
	}
	return (1);
}
