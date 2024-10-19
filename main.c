/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:05 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/18 16:55:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "./includes/minirt.h"


int main(int argc, char **argv)
{
    t_map *map;

    if (argc != 2)
        return (printf("error : try with : ./minirt scene.rt\n"), 1);
    map = (t_map *)malloc(sizeof(t_map));
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_sen_map(argv[1]);
    if (!map->lines || ft_check_map_components(&map))
        return (free(map),1);
    var_dump_lines(map->lines);
    return (0);
}