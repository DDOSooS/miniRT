/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 18:25:05 by aghergho          #+#    #+#             */
/*   Updated: 2024/10/19 16:16:30 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include  "./includes/minirt.h"


int main(int argc, char **argv)
{
    t_map *map;

    if (argc != 2)
        return (printf("error : try with : ./minirt scene.rt\n"), 1);
    // printf
    map = (t_map *)malloc(sizeof(t_map));
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_sen_map(argv[1]);
    if (!map->lines || !ft_check_map_components(&map))
    {
        printf("error at map components: \n");
        return (free(map),1);
    }
    var_dump_lines(map->lines);
    printf("donee");
    return (0);
}