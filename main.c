/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:30:26 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:22:27 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minirt.h"

t_map	*ft_init_map(void)
{
	t_map	*new;
	int		i;

	i = -1;
	new = (t_map *)malloc(sizeof(t_map));
	if (!new)
		return (NULL);
	new->lines = NULL;
	while (++i < 3)
		new->scen_elements[i] = 0;
	return (new);
}

int	ft_close_window(t_scene *scen)
{
	ft_destroy_scene(scen);
	ft_free_map(&scen->map->lines);
	free(scen->map);
	mlx_destroy_image(scen->data->mlx, scen->data->img.img_ptr);
	mlx_destroy_window(scen->data->mlx, scen->data->win);
	mlx_destroy_display(scen->data->mlx);
	free(scen->data->mlx);
	free(scen->data);
	free(scen);
	exit(EXIT_SUCCESS);
	return (1);
}

void	init_scene(t_scene *scene)
{
	scene->data->mlx = mlx_init();
	scene->data->win = mlx_new_window(scene->data->mlx, SCREEN_WIDTH,
			SCREEN_HEIGHT, "MiniRT");
	scene->data->img.img_ptr = mlx_new_image(scene->data->mlx, SCREEN_WIDTH,
			SCREEN_HEIGHT);
	scene->data->img.addr = mlx_get_data_addr(scene->data->img.img_ptr,
			&scene->data->img.bits_per_pixel, &scene->data->img.line_length,
			&scene->data->img.endian);
}

t_scene	*allocate_scene(void)
{
	t_scene	*scene;

	scene = malloc(sizeof(t_scene));
	if (!scene)
		return (NULL);
	scene->data = malloc(sizeof(t_var));
	if (!scene->data)
		return (NULL);
	scene->sphere = NULL;
	scene->plane = NULL;
	scene->cylinder = NULL;
	scene->cone = NULL;
	scene->light = NULL;
	return (scene);
}

/*main function*/
int	main(int argc, char **argv)
{
	t_map	*map;
	t_scene	*scene;

	if (argc != 2)
		return (printf("error: try with : ./minirt scene.rt\n"), 1);
	map = ft_init_map();
	if (!map)
		return (printf("error : memory allocation failed\n"), 1);
	map->lines = ft_gen_scen_map(argv[1]);
	if (!map->lines || !ft_check_map_components(&map))
		return (free(map), ft_putstr_fd("map is empty\n", 2), 1);
	scene = allocate_scene();
	init_scene(scene);
	if (!ft_generate_scene(map->lines, &scene))
		return (0);
	scene->map = map;
	scene->world = default_world(scene);
	render_image(scene, scene->camera);
	return (0);
}
