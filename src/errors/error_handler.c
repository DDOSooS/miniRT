/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 02:10:46 by aghergho          #+#    #+#             */
/*   Updated: 2025/03/17 02:22:11 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

void	error_handler(int i, int line, char *identifier)
{
	if (i == 0)
	{
		printf("line %d: \n", line);
		printf("identifier is not a valid identifier (%s)\n", identifier);
	}
	else if (i == 1)
		printf("Error at map checking line component at line %d \n", line);
	else if (i == 2)
		printf("duplicated of elements that must be declared just Once\n");
}
