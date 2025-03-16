/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 09:25:34 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/16 15:39:46 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minirt.h"

float	**convert_to_pointer_array(float matrix[4][4])
{
	float	**result;
	int		i;
	int		j;

	i = 0;
	result = (float **)malloc(4 * sizeof(float *));
	while (i < 4)
	{
		j = 0;
		result[i] = (float *)malloc(4 * sizeof(float));
		while (j < 4)
		{
			result[i][j] = matrix[i][j];
			j++;
		}
		i++;
	}
	return (result);
}

t_ray	transform_ray(t_ray ray, float **transform)
{
	t_ray	transformed_ray;

	ray.origin.w = 1;
	transformed_ray.origin = matrix_multiply_vector(transform, ray.origin);
	ray.direction.w = 0;
	transformed_ray.direction = matrix_multiply_vector(transform,
			ray.direction);
	transformed_ray.direction.w = 0;
	return (transformed_ray);
}

/* main function */
float	**create_translation_matrix(t_cone *cone)
{
	float	**transform;

	transform = create_identity_matrix(4);
	transform[0][3] = -cone->apex.x;
	transform[1][3] = -cone->apex.y;
	transform[2][3] = -cone->apex.z;
	return (transform);
}

void	init_rotation_matric(float rotation_matrix[4][4])
{
	rotation_matrix[0][0] = 1;
	rotation_matrix[0][1] = 0;
	rotation_matrix[0][2] = 0;
	rotation_matrix[0][3] = 0;
	rotation_matrix[1][0] = 0;
	rotation_matrix[1][1] = -1;
	rotation_matrix[1][2] = 0;
	rotation_matrix[1][3] = 0;
	rotation_matrix[2][0] = 0;
	rotation_matrix[2][1] = 0;
	rotation_matrix[2][2] = -1;
	rotation_matrix[2][3] = 0;
	rotation_matrix[3][0] = 0;
	rotation_matrix[3][1] = 0;
	rotation_matrix[3][2] = 0;
	rotation_matrix[3][3] = 1;
}

float	**handle_opposite_up_case(float **transform)
{
	float	rotation_matrix[4][4];
	float	**rotation_matrix_ptr;
	float	**result;
	int		i;

	i = 0;
	init_rotation_matric(rotation_matrix);
	rotation_matrix_ptr = convert_to_pointer_array(rotation_matrix);
	result = matrix_multiply(transform, rotation_matrix_ptr);
	while (i < 4)
	{
		free(rotation_matrix_ptr[i]);
		i++;
	}
	free(rotation_matrix_ptr);
	free_matrix(transform);
	return (result);
}

float	**apply_rotation(float **transform, t_vector cross, float angle)
{
	float	rotation_matrix[4][4];
	float	**rotation_matrix_ptr;
	float	**result;
	int		i;

	create_rotation_matrix(cross, angle, rotation_matrix);
	rotation_matrix_ptr = convert_to_pointer_array(rotation_matrix);
	result = matrix_multiply(transform, rotation_matrix_ptr);
	i = 0;
	while (i < 4)
	{
		free(rotation_matrix_ptr[i]);
		i++;
	}
	free(rotation_matrix_ptr);
	free_matrix(transform);
	return (result);
}

float	**create_rotation_matrix_from_vector_cone(t_vector axis)
{
    float **rotation;
    float temp_matrix[4][4];
    t_transform_vars vars;
    
    rotation = create_identity_matrix(4);
    vars.axis = vector_normilze(axis);
    vars.up = (t_vector){0, 1, 0, 0};
    
    if (fabs(vars.axis.x) < 0.0001 && fabs(vars.axis.y + 1) < 0.0001 && fabs(vars.axis.z) < 0.0001)
    {
        init_rotation_matric(temp_matrix);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                rotation[i][j] = temp_matrix[i][j];
    }
    else
    {
        vars.dot = vector_dot(vars.axis, vars.up);
        vars.angle = acos(vars.dot);
        vars.cross = vector_cross(vars.up, vars.axis);
        
        if (fabs(vars.angle) > 0.0001)
        {
            create_rotation_matrix(vars.cross, vars.angle, temp_matrix);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    rotation[i][j] = temp_matrix[i][j];
        }
    }
    
    return rotation;
}


float	**create_cone_transform(t_cone *cone)
{
	float **rotation;
	float **translation;
	float **combined;

    rotation = create_rotation_matrix_from_vector_cone(cone->axis);
    translation = create_translation_matrix(cone);
    combined = matrix_multiply(rotation, translation);
    free_matrix(rotation);
    free_matrix(translation);
    return (combined);
}

/* main function */
t_ray	get_local_ray(t_ray ray, t_cone *cone)
{
	float	**transform;
	t_ray	local_ray;

	transform = create_cone_transform(cone);
	local_ray = transform_ray(ray, transform);
	free_matrix(transform);
	return (local_ray);
}

/* main function */
void	calculate_cone_coefficients(t_ray local_ray, t_cone *cone,
	t_quadratic *coeff)
{
	t_vector	oc;
	float		k;
	float		k2;

	oc = vector_sub(local_ray.origin, (t_vector){0, 0, 0, 0});
	k = cone->radius / cone->height;
	k2 = k * k;
	coeff->a = local_ray.direction.x * local_ray.direction.x
		+ local_ray.direction.z * local_ray.direction.z
		- k2 * local_ray.direction.y * local_ray.direction.y;
	coeff->b = 2 * (local_ray.direction.x * oc.x
			+ local_ray.direction.z * oc.z
			- k2 * local_ray.direction.y * oc.y);
	coeff->c = oc.x * oc.x + oc.z * oc.z - k2 * oc.y * oc.y;
}

int	is_point_in_cone_bounds(float y, float height)
{
	return (y >= 0 && y <= height);
}

/* main function */
void	calculate_intersection_points(t_ray local_ray, t_cone *cone,
	t_intdata t_values, t_intdata *valid_t)
{
	float	y1;
	float	y2;

	y1 = local_ray.origin.y + t_values.t1 * local_ray.direction.y;
	y2 = local_ray.origin.y + t_values.t2 * local_ray.direction.y;
	if (is_point_in_cone_bounds(y1, cone->height))
		valid_t->t1 = t_values.t1;
	else
		valid_t->t1 = -1;
	if (is_point_in_cone_bounds(y2, cone->height))
		valid_t->t2 = t_values.t2;
	else
		valid_t->t2 = -1;
}

void	init_intersection(t_intersection *inter)
{
	inter->n_sol = 0;
	inter->t1 = 0;
	inter->object = NULL;
	inter->type = 0;
}

/* main function */
t_intersection	ft_intersect_cone(t_ray ray, t_cone *cone)
{
	t_intersection	inter;
	t_quadratic		coeff;
	t_intdata		ts;
	t_ray			local_ray;
	float			discriminant;

	init_intersection(&inter);
	local_ray = get_local_ray(ray, cone);
	calculate_cone_coefficients(local_ray, cone, &coeff);
	discriminant = coeff.b * coeff.b - 4 * coeff.a * coeff.c;
	if (discriminant < 0)
		return (inter);
	discriminant = sqrt(discriminant);
	ts.t1 = (-coeff.b - discriminant) / (2 * coeff.a);
	ts.t2 = (-coeff.b + discriminant) / (2 * coeff.a);
	calculate_intersection_points(local_ray, cone, ts, &ts);
	if (ts.t1 >= 0 || ts.t2 >= 0)
	{
		inter.n_sol = 2;
		inter.t1 = ts.t1;
		inter.object = cone;
		inter.type = SHAPE_CONE;
	}
	return (inter);
}
