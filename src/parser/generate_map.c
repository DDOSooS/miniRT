/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 01:49:32 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 10:21:15 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_map_line	*ft_get_last_line(t_map_line **map)
{
	t_map_line	*tmp;

	tmp = *map;
	if (!tmp)
		return (NULL);
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

int	ft_add_line(t_map_line **map, char *line)
{
	t_map_line	*new;
	t_map_line	*last;

	new = (t_map_line *)malloc(sizeof(t_map_line));
	if (!new)
		return (0);
	new->line_component = ftt_split(line, " \t\n\r\v");
	new->next = NULL;
	last = ft_get_last_line(map);
	if (last)
		last->next = new;
	else
		*map = new;
	return (1);
}

int	file_cheker(char *file_name, int *fd)
{
	if (!ft_check_file_name(file_name))
		return (printf("error : invalid file extension\n"), 0);
	*fd = open(file_name, O_RDONLY);
	if (*fd < 0)
		return (printf("error : failed to open file\n"), 0);
	return (1);
}

t_map_line	*ft_gen_scen_map(char *file_name)
{
	t_map_line	*map;
	char		*line;
	int			fd;

	if (!file_cheker(file_name, &fd))
		return (NULL);
	map = NULL;
	line = get_next_line(fd);
	while (line)
	{
		if (line && !is_empty_line(line))
		{
			if (line && line[0] == '#')
			{
				free(line);
				line = get_next_line(fd);
				continue ;
			}
			if (!ft_add_line(&map, line))
				return (free(line), close(fd), NULL);
		}
		free(line);
		line = get_next_line(fd);
	}
	return (free(line), close(fd), map);
}
