/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 20:24:10 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 01:48:29 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../includes/minirt.h"
#include <string.h>


int ft_gen_elements(t_vector *coordinates, char *components)
{
	char **tmp_cord;

	tmp_cord = split(components, ",");
	if (!tmp_cord)
		return (0);
	*coordinates = ft_new_vector(ft_atod(tmp_cord[0]),ft_atod(tmp_cord[1]), ft_atod(tmp_cord[2]));
	ft_free_line_components(tmp_cord);
	return (1);
}

//to do norm
int get_texture(t_sphere *sphere, char *texture_name, t_scene **scene)
{
	int fd = open(texture_name, O_RDONLY);
	if (fd < 0)
	{
		perror("Error: Failed to open texture file");
		return (0);
	}
	close(fd);

	sphere->texture = malloc(sizeof(t_texture));
	if (!sphere->texture)
	{
		perror("Error: Failed to allocate memory for texture");
		return (0);
	}

	sphere->texture->img_ptr = mlx_xpm_file_to_image(
		(*scene)->data->mlx,
		texture_name,
		&sphere->texture->width,
		&sphere->texture->height
	);
	if (!sphere->texture->img_ptr)
	{
		perror("Error: Failed to load XPM image");
		free(sphere->texture);
		sphere->texture = NULL;
		return (0);
	}
	sphere->texture->img_data = mlx_get_data_addr(
		sphere->texture->img_ptr,
		&sphere->texture->bpp,
		&sphere->texture->size_line,
		&sphere->texture->endian
	);
	if (!sphere->texture->img_data)
	{
		perror("Error: Failed to get image data");
		mlx_destroy_image(NULL, sphere->texture->img_ptr);
		free(sphere->texture);
		sphere->texture = NULL;
		return(0);
	}
	return (1);
}

int ft_add_component(t_scene **scene, int identifier, char **components)
{
	if (identifier == 1)
		return (ft_add_ambient(scene, components));
	if (identifier == 2)
		return (ft_add_camera(scene, components));
	if (identifier == 3)
		return (ft_add_light(scene, components));
	if (identifier == 4)
		return (ft_add_sphere(scene, components));
	if (identifier == 5)
		return (ft_add_plane(scene, components));
	if (identifier == 6)
		return (ft_add_cylinder(scene, components));
	if (identifier == 7)
		return (ft_add_cone(scene, components));
	return (1);
}
 
int ft_generate_scene(t_map_line *compoenent, t_scene **scene)
{
	int     identifier_id;

	while (compoenent)
	{
		identifier_id = is_identifier(compoenent->line_component[0]);
		if (identifier_id)
			ft_add_component(scene, identifier_id, compoenent->line_component);
		compoenent = compoenent->next;
	}
	return 1;
}