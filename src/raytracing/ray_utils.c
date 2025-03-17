/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aghergho <aghergho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 12:52:52 by mkartit           #+#    #+#             */
/*   Updated: 2025/03/17 02:42:24 by aghergho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minirt.h"

t_ray	create_ray(t_point origin, t_vector dir)
{
	t_ray	new;

	new.origin = origin;
	new.direction = dir;
	return (new);
}

t_point	position(t_ray ray, float distance)
{
	t_point	new;

	new.x = ray.origin.x + ray.direction.x * distance;
	new.y = ray.origin.y + ray.direction.y * distance;
	new.z = ray.origin.z + ray.direction.z * distance;
	new.w = 1.0;
	return (new);
}

t_intersection	ray_hit(t_intersection *inters, int count)
{
	float min_t = (float)INT_MAX;
	t_intersection result;
	
	t_intersection *tmp = &inters[0];
	int i = 0;
	
	while (i < count)
	{
		if (inters[i].t1 > 0 && inters[i].t1 < min_t)
		{
			min_t = inters[i].t1;
			tmp = &inters[i];
		}
		i++;
	}
	result.n_sol = tmp->n_sol;
	result.t1 = tmp->t1;
	result.object = tmp->object;
	result.type = tmp->type;
	return result;
}

float calculate_discriment(t_ray ray, t_sphere *sphere)
{
	float a, b, c;
	t_vector sphere_to_ray;

	sphere_to_ray = vector_sub(ray.origin, sphere->sphere_coordinates);
	a = vector_dot(ray.direction, ray.direction);
	b = 2 * vector_dot(ray.direction, sphere_to_ray);
	c = vector_dot(sphere_to_ray, sphere_to_ray) - (sphere->sphere_diameter/2.0 * sphere->sphere_diameter/2.0);
	return b * b - 4 * a * c;
}



t_intersection ft_intersect_sphere(t_ray ray, t_sphere *sphere) {
	t_intersection result;
	float a, b, discriminant;
	float t1, t2; 
	t_vector sphere_to_ray;
	float closest_t;

	sphere_to_ray = vector_sub(ray.origin, sphere->sphere_coordinates);
	a = vector_dot(ray.direction, ray.direction);
	b = 2 * vector_dot(ray.direction, sphere_to_ray);
	discriminant = calculate_discriment(ray, sphere);
	result.n_sol = 0;
	result.t1 = -1;
	if (discriminant >= 0)
	{
		t1 = (-b - sqrtf(discriminant)) / (2 * a);
		t2 = (-b + sqrtf(discriminant)) / (2 * a);
		closest_t = -1;
		if (t1 > EPSILON) 
			closest_t = t1;        
		if (t2 > EPSILON && (closest_t < 0 || t2 < closest_t))
			closest_t = t2;
		if (closest_t > 0)
		{
			result.t1 = closest_t;
			result.n_sol = 2;
			result.object = sphere;
			result.type = SHAPE_SPHERE;
		}
	}
	return result;
}

t_intersection  ft_new_intersection(float t, void *object, int type)
{
	t_intersection intersection;

	intersection.t1 = t;
	intersection.object = object;
	intersection.type = type;
	return (intersection);
}

t_ray transform(t_ray ray, float **m)
{
	t_vector dir;
	t_point origin;

	dir = ft_multiply_matrix_vec(m, ray.direction);
	origin = ft_multiply_matrix_vec(m, ray.origin);
	return create_ray(origin, dir);
}

t_sphere *default_sphere()
{
	t_sphere *sphere;

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
	   return NULL;
	sphere->sphere_coordinates = ft_new_point(0,0,0);
	sphere->sphere_diameter =1;
	sphere->transform = identity_matrix(4);
	sphere->material = default_material();
	sphere->next = NULL;
	return sphere;
}

t_shape *ft_new_shape(void *shape_obj, int shape_type)
{
	t_shape *new_shape = malloc(sizeof(t_shape));

	if (!new_shape)
		return NULL;
	new_shape->type = shape_type;
	new_shape->next = NULL;
	if (shape_type == SHAPE_SPHERE) 
		new_shape->u_objects.sphere = (t_sphere *)shape_obj;
	else if (shape_type == SHAPE_PLANE)
		new_shape->u_objects.plane = (t_plane *)shape_obj;
	else if (shape_type == SHAPE_CYLINDER)
		new_shape->u_objects.cylinder = (t_cylinder *)shape_obj;
	else if (shape_type == SHAPE_CONE)
		new_shape->u_objects.cone = (t_cone *)shape_obj;
	else
	{
		free(new_shape);
		return NULL;
	}
	return new_shape;
}

void ft_add_shape(t_world **root, void *new, int shape)
{

	if (!(*root)->shape)
	{
		(*root)->shape = ft_new_shape(new, shape);
		(*root)->n_objects++;
	}
	else
	{
		t_shape *current = (*root)->shape;
		while (current->next)
			current = current->next;
		current->next = ft_new_shape(new, shape);
		(*root)->n_objects++;
	}
}

t_intersection ft_intersect_plane(t_ray ray, t_plane *plane)
{
	t_intersection result = {0,  0, NULL, 0};
	
	// Calculate denominator (dot product of ray direction and plane normal)
	float denom = vector_dot(ray.direction, plane->plane_normal);
	// Check for parallel ray (denominator near zero)
	if (fabs(denom) < EPSILON)  
		return result;
	// Calculate vector from ray origin to a point on the plane
	t_vector origin_to_plane = vector_sub(ray.origin, plane->plane_cordinates); 
	// Fixed: use plane->point instead of plane->plane_normal
	//Calculate intersection distance
	float t = -(vector_dot(origin_to_plane, plane->plane_normal)) / denom;

	if (t < EPSILON)
	{

		return result;  // Intersection is behind ray origin
	}
	// printf("INTERSECTION IS BEING FOUND\n");              
	result.n_sol =1;
	result.t1 = t;
	result.object = plane;
	result.type = SHAPE_PLANE;
	// printf(" intersection with plane is being made\n");
	return result;
}

int check_cylinder_caps(t_ray *ray, float t, float radius)
{
	float x,z;
	
	x = ray->origin.x  + t * ray->direction.x;
	z = ray->origin.z  + t * ray->direction.z;
	return (x*x + z*z <= radius);
}

t_intersection check_intersection_caps(t_ray ray, t_cylinder *cylinder, float cap)
{
	t_intersection result = {-1, -1, NULL, SHAPE_CYLINDER};
	float xs ;
	float x ;
	float z;
	float radius_squared ;

	//unitialided values
	if (fabs(ray.direction.y) < EPSILON)
		return result; 
	xs= (cap - ray.origin.y) / ray.direction.y;
	if (xs < EPSILON)
		return result;
	x = ray.origin.x + xs * ray.direction.x;
	z = ray.origin.z + xs * ray.direction.z;
	radius_squared = (cylinder->raduis + EPSILON) * (cylinder->raduis + EPSILON);
	if ((x * x + z * z) <= radius_squared)
	{
		result.t1 = xs;
		result.n_sol = 1;
		result.object = cylinder;
	}
	
	return result;
}

float get_min_sol(float *arr, int len)
{
	int i;
	float min;
	i = 1;
	min = arr[0];
	while (i < len)
	{
		if (min > arr[i])
			min = arr[i];
		i++;
	}
	return min;
}

int  is_point_inside_cylinder(t_point point, t_cylinder *cylinder)
{
	float **rotation;
	float **combined;
	float **inverse;
	float half_height, radial_dist;
	t_vector tr_p;
	
	rotation  = create_rotation_matrix_from_vector(cylinder->orientation);
	combined = ft_multiply_matrix(cylinder->transform, rotation, 4, 4);
	inverse = inverse_matrix(combined, 4);
	if (!inverse)
		inverse = cylinder->transform;
	tr_p = ft_multiply_matrix_vec(inverse, point);
	half_height = cylinder->height / 2.0f;
	ft_free_matrix(rotation, 4);
	ft_free_matrix(combined, 4);
	if (inverse != cylinder->transform)
		ft_free_matrix(inverse, 4);
	if (tr_p.y > half_height || tr_p.y < -half_height)
		return 0;
	radial_dist = sqrtf(tr_p.x * tr_p.x + tr_p.z * tr_p.z);
	return radial_dist < cylinder->raduis;
}

void create_rotation_matrix(t_vector axis, float angle, float rotation_matrix[4][4])
{
	float cos_theta = cos(angle);
	float sin_theta = sin(angle);
	float one_minus_cos = 1.0f - cos_theta;

	// Normalize the axis
	float length = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	axis.x /= length;
	axis.y /= length;
	axis.z /= length;

	// Create the rotation matrix
	rotation_matrix[0][0] = cos_theta + axis.x * axis.x * one_minus_cos;
	rotation_matrix[0][1] = axis.x * axis.y * one_minus_cos - axis.z * sin_theta;
	rotation_matrix[0][2] = axis.x * axis.z * one_minus_cos + axis.y * sin_theta;
	rotation_matrix[0][3] = 0.0f;

	rotation_matrix[1][0] = axis.y * axis.x * one_minus_cos + axis.z * sin_theta;
	rotation_matrix[1][1] = cos_theta + axis.y * axis.y * one_minus_cos;
	rotation_matrix[1][2] = axis.y * axis.z * one_minus_cos - axis.x * sin_theta;
	rotation_matrix[1][3] = 0.0f;

	rotation_matrix[2][0] = axis.z * axis.x * one_minus_cos - axis.y * sin_theta;
	rotation_matrix[2][1] = axis.z * axis.y * one_minus_cos + axis.x * sin_theta;
	rotation_matrix[2][2] = cos_theta + axis.z * axis.z * one_minus_cos;
	rotation_matrix[2][3] = 0.0f;

	rotation_matrix[3][0] = 0.0f;
	rotation_matrix[3][1] = 0.0f;
	rotation_matrix[3][2] = 0.0f;
	rotation_matrix[3][3] = 1.0f;
}

float **create_matrix(int rows, int cols)
{
	float   **m;
    int     i;

    i = -1;
    m = malloc(sizeof(float *) * rows);
	while (++i < rows)
		m[i] = malloc(sizeof(float) * cols);
	return m;
}

float **create_identity_matrix(int size)
{
	float   **m;
    int     i;
    int     j;

    i = -1;
    m = create_matrix(size, size);
	while (++i < size)
	{
        j = -1;
		while (++j < size)
        {
            if (i == j)
                m[i][j] = 1;
            else
                m[i][j] = 0;
        }
	}
	return m;
}

float **matrix_multiply(float **a, float **b)
{
	float **result;
    int     i;
    int     j;
    int     k;

    i = -1;
    result = create_matrix(4, 4);
	while (++i < 4)
	{
        j = -1;
		while(++j < 4)
		{
            result[i][j] = 0;
            k = -1;
            while (++k < 4)
				result[i][j] += a[i][k] * b[k][j];
		}
	}
	return result;
}

void free_matrix(float **m)
{
    int i;

    i = -1;
	while (++i < 4)
		free(m[i]);
	free(m);
}

t_vector matrix_multiply_vector(float **m, t_vector v)
{
	t_vector result;

	result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w;
	result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w;
	result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w;
	result.w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w;
	return result;
}

float **rotation_matrix(t_vector axis, float angle)
{
	float **m = create_matrix(4, 4);
	float c = cos(angle);
	float s = sin(angle);
	float t = 1 - c;

	axis = vector_normilze(axis);

	m[0][0] = c + axis.x * axis.x * t;
	m[0][1] = axis.x * axis.y * t - axis.z * s;
	m[0][2] = axis.x * axis.z * t + axis.y * s;
	m[0][3] = 0;

	m[1][0] = axis.y * axis.x * t + axis.z * s;
	m[1][1] = c + axis.y * axis.y * t;
	m[1][2] = axis.y * axis.z * t - axis.x * s;
	m[1][3] = 0;

	m[2][0] = axis.z * axis.x * t - axis.y * s;
	m[2][1] = axis.z * axis.y * t + axis.x * s;
	m[2][2] = c + axis.z * axis.z * t;
	m[2][3] = 0;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	return m;
}

float **translation_matrix(float x, float y, float z)
{
	float **m = create_matrix(4, 4);
	m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = x;
	m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = y;
	m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = z;
	m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
	return m;
}

// NORMINETTE URGENT TO DO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
t_intersection ft_intersect_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_intersection result = {-1, -1, NULL, SHAPE_CYLINDER};
	t_intersection captop, capbottom;
	float half_height = cylinder->height / 2.0f;
	float sol[4];
	int counter = 0;
	int is_inside = 0;
	float	**inverse;

    //  rotation matrix && orientation vector
	inverse = get_combined_inv(cylinder);
    // Transform ray to object space
    t_ray tr_ray = create_ray(ft_multiply_matrix_vec(inverse, ray.origin),
                                        ft_multiply_matrix_vec(inverse, ray.direction));
    float a = tr_ray.direction.x * tr_ray.direction.x + tr_ray.direction.z * tr_ray.direction.z;
    float b = 2.0f * (tr_ray.origin.x * tr_ray.direction.x + tr_ray.origin.z * tr_ray.direction.z);
    float c = tr_ray.origin.x * tr_ray.origin.x + tr_ray.origin.z * tr_ray.origin.z - cylinder->raduis/2.f * cylinder->raduis/2.f;
    float discriminant = b * b - 4.0f * a * c;
    float disance = INFINITY;
    if (discriminant >= 0)
    {
        float sqrt_disc = sqrtf(discriminant);
        float t1 = (-b - sqrt_disc) / (2.0f * a);
        float t2 = (-b + sqrt_disc) / (2.0f * a);
        float y1 = tr_ray.origin.y + t1 * tr_ray.direction.y;
        float y2 = tr_ray.origin.y + t2 * tr_ray.direction.y;
        if (y1 >= -half_height && y1 <= half_height)
            sol[counter++] = t1;
        if (y2 >= -half_height && y2 <= half_height) 
            sol[counter++] = t2;
    }
    captop = check_intersection_caps(tr_ray, cylinder, half_height);
    if (captop.n_sol > 0)
        sol[counter++] = captop.t1;
    capbottom = check_intersection_caps(tr_ray, cylinder, -half_height);
    if (capbottom.n_sol > 0)
        sol[counter++] = capbottom.t1;
    if (counter > 0)
    {
        float min_t = get_min_sol(sol, counter);
        t_point p;
        p.x = ray.origin.x * min_t + ray.direction.x; 
        p.y = ray.origin.y * min_t + ray.direction.y; 
        p.z = ray.origin.z * min_t + ray.direction.z;
        p.w = ray.origin.w * min_t * ray.direction.w; 
        is_inside = is_point_inside_cylinder(p, cylinder);
        if (min_t > EPSILON || is_inside)
        {
            result.n_sol = 1;
            result.object = cylinder;
            result.t1 = min_t;
        }
    }
	ft_free_matrix(inverse, 4);
    return result;
}

t_vector vector_mult_scalar(t_vector v, float scalar)
{
	t_vector result;
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	result.w = v.w * scalar;
	return result;
}



float **copy_matrix(float **m)
{
    float   **res;
    int     i;
    int     j;

    res = ft_create_matrix(4,4);
    i = -1;
    while (++i < 4)
    {
        j = -1;
        while (++j < 4)
            res[i][j] = m[i][j];
    }
    return res;
}

float **get_combined_inv(t_cylinder *cy)
{
    float **rotation;
    float **combined;
	float **inv;

    rotation = create_rotation_matrix_from_vector(cy->orientation);
    combined = ft_multiply_matrix(cy->transform, rotation, 4, 4);
    inv = inverse_matrix(combined, 4);
    if (!inv)
        inv = copy_matrix(cy->transform);
    ft_free_matrix(rotation, 4);
    ft_free_matrix(combined, 4);
    return inv;
}

t_vector normalize_at_cylinder_pos(t_cylinder *cylinder, t_point world_p)
{
	float **inv;
	t_vector world_normal;
	t_point local_p;
    
	//  normal in object space
    inv = get_combined_inv(cylinder);
    local_p = ft_multiply_matrix_point(inv, world_p);
	t_vector local_normal;
	if (fabs(local_p.y - cylinder->height/2) < EPSILON)
		local_normal = (t_vector){0, 1, 0};
	else if (fabs(local_p.y + cylinder->height/2) < EPSILON)
		local_normal = (t_vector){0, -1, 0};
	else
		local_normal = (t_vector){local_p.x, 0, local_p.z};
	ft_transpose_matrix(&inv, 4, 4);
	world_normal = ft_multiply_matrix_vec(inv, local_normal);
    ft_free_matrix(inv, 4);
	return vector_normilze(world_normal);
}




t_intersection intersect_world(t_world *world, t_ray ray)
{
	t_intersection  res;
	t_intersection  *inter;
	t_shape         *current;
	int i;

	if (!world->shape)
		return (t_intersection) {0,0,NULL,0};
	current = world->shape;
	inter = malloc(sizeof(t_intersection) * world->n_objects);
	i = -1;
	while (current)
	{
		if (current->type == SHAPE_SPHERE)
			inter[++i] = ft_intersect_sphere(ray, current->u_objects.sphere);
		else if (current->type == SHAPE_PLANE)
			inter[++i] = ft_intersect_plane(ray, current->u_objects.plane);
		else if (current->type == SHAPE_CYLINDER)
			inter[++i] = ft_intersect_cylinder(ray, current->u_objects.cylinder);  
		else if (current->type == SHAPE_CONE)
			inter[+i] = ft_intersect_cone(ray, current->u_objects.cone);
		current = current->next;
	}
	res = ray_hit(inter, world->n_objects);
	return (free(inter), res);
}

t_vector normilize_at_sphere_pos(t_sphere *sphere, t_point w_p)
{
	t_vector vec;
	t_vector obj_n;
	t_point obj_p;
	t_vector tmp;
	float **inv_m;

	inv_m = inverse_matrix(sphere->transform, 4);
	if (!inv_m)
		inv_m = sphere->transform;
	obj_p = ft_multiply_matrix_vec(inv_m, w_p);
	obj_n = vector_sub(w_p, sphere->sphere_coordinates);
	ft_transpose_matrix(&inv_m, 4,4);
	tmp = ft_multiply_matrix_vec(inv_m, obj_n);
	tmp.w = 0;
	vec = vector_normilze(tmp);
	if (inv_m != sphere->transform)
		ft_free_matrix(inv_m, 4);
	return vec;
}

/*
linear algebraic transformation
(M^-1)^T * N
*/
t_vector normalize_at_plane_pos(t_plane *plane, t_point w_p)
{
	t_vector    world_normal;
	float       **inv_transpose;
	t_vector    normalized;
	t_vector    obj_normal;
	
	obj_normal = plane->plane_normal;
		inv_transpose = inverse_matrix(plane->transform, 4);
	if (!inv_transpose)
		inv_transpose = plane->transform;
	ft_transpose_matrix(&inv_transpose, 4, 4);
	world_normal = ft_multiply_matrix_vec(inv_transpose, obj_normal);
	world_normal.w = 0;  
	normalized = vector_normilze(world_normal);    
	if (inv_transpose != plane->transform)
		ft_free_matrix(inv_transpose, 4);
	return normalized;
}

t_point point_add(t_point point, t_vector p2)
{
	t_point new_point;

	new_point.x = point.x + p2.x;
	new_point.y = point.y + p2.y;
	new_point.z = point.z + p2.z;
	new_point.w = 1.0;  
	return new_point;
}

t_compose *prepare_computations(t_intersection inter, t_ray ray)
{
	t_compose *comp = malloc(sizeof(t_compose));

	comp->t = inter.t1;
	comp->obj = inter.object;
	comp->obj_type = inter.type;
	comp->camv = negate_vector(ray.direction);
	comp->point = position(ray, comp->t);
	if (comp->obj_type == SHAPE_SPHERE)
		comp->normv = normilize_at_sphere_pos((t_sphere *)(inter.object), comp->point);
	else if (comp->obj_type == SHAPE_PLANE)
		comp->normv = normalize_at_plane_pos((t_plane *)(inter.object), comp->point);
	else if (comp->obj_type == SHAPE_CYLINDER)
		comp->normv = normalize_at_cylinder_pos((t_cylinder *)(inter.object), comp->point);
	else if (comp->obj_type == SHAPE_CONE)
		comp->normv = normalize_at_cone_pos((t_cone *)(inter.object), comp->point);
	comp->normv = vector_normilze(comp->normv);
	if (vector_dot(comp->normv, comp->camv) < 0.0)
		comp->normv = negate_vector(comp->normv);
	comp->over_point = point_add(comp->point, ft_scale_point(comp->normv, EPSILON));
	return comp;
}

/*
	X-axis corresponds to left/right
	Y-axis corresponds to up/down
	Z-axis corresponds to forward/backward (with negative Z being forward)
*/

//camera transform matrix that transform from world coordinates to camera system Co




//y rotation than x rotation~~~~~~
float **create_rotation_matrix_from_vector(t_vector orientation)
{
    t_vector new_y = vector_normilze(orientation);
    t_vector tmp_axis;
    t_vector new_x, new_z;
    
    // Choose a temporary axis that's not parallel to new_y
    if (fabs(new_y.y) < 0.9999f)
        tmp_axis = (t_vector){0, 1, 0}; // Use default up (Y-axis)
    else
        tmp_axis = (t_vector){0, 0, 1}; // If new_y is close to Y, use Z-axis
    
    new_x = vector_normilze(vector_cross(tmp_axis, new_y));
    new_z = vector_normilze(vector_cross(new_y, new_x));
    // Create the 4x4 rotation matrix
    float **matrix = create_matrix(4, 4);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
    matrix[0][0] = new_x.x; matrix[0][1] = new_y.x; matrix[0][2] = new_z.x;
    matrix[1][0] = new_x.y; matrix[1][1] = new_y.y; matrix[1][2] = new_z.y;
    matrix[2][0] = new_x.z; matrix[2][1] = new_y.z; matrix[2][2] = new_z.z;
    return matrix;
}
/*
*/
void ft_add_cylinder_shape(t_world *world, t_cylinder *cylinder)
{
	t_cylinder *tmp;

	tmp = cylinder;
	while (tmp)
	{
		if (tmp->orientation.x != 0 ||  tmp->orientation.y != 0 || tmp->orientation.z != 0)
			ft_add_shape(&world,tmp, SHAPE_CYLINDER);
		tmp = tmp->next;
	}
}

void ft_add_plane_shape(t_world *world, t_plane *plane)
{
	t_plane *tmp;

	tmp = plane;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_PLANE);
		tmp = tmp->next;
	}
}

void ft_add_sphere_shape(t_world *world, t_sphere *sphere)
{
	t_sphere *tmp;

	tmp = sphere;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_SPHERE);
		tmp = tmp->next;
	}
}

void	ft_add_cone_shape(t_world *world, t_cone *cone)
{
	t_cone	*tmp;

	tmp = cone;
	while (tmp)
	{
		ft_add_shape(&world, tmp, SHAPE_CONE);
		tmp = tmp->next;
	}
}

void ft_add_plight(t_world *world, t_light *light)
{
	t_light *tmp;
	
	world->light = light;
	tmp = light;
	while(tmp)
	{
		tmp->color = ft_scale_color(tmp->color, tmp->ration);
		tmp = tmp->next;
	}
}

t_world	*default_world(t_scene *scene)
{
	t_world *world;

	world = malloc(sizeof(t_world));
	if (!world)
		return NULL;
	world->n_objects = 0;
	world->shape = NULL;
	if (scene->cylinder)
		ft_add_cylinder_shape(world, scene->cylinder);
	if (scene->plane)
		ft_add_plane_shape(world, scene->plane);
	if (scene->sphere)
		ft_add_sphere_shape(world, scene->sphere);
	if (scene->cone)
		ft_add_cone_shape(world, scene->cone);
	ft_add_plight(world, scene->light);
	return (world);
}



t_ray get_ray_pixel(t_scamera *cam, float x, float y, float edge)
{
	t_ray ray;
	t_point pixel_world;
	t_point pixel;
	float world_x, world_y;
	float **inv;

	world_x = cam->half_w_size - (x + edge + EPSILON) * cam->pixel_size;
	world_y = cam->half_h_size - (y + edge + EPSILON) * cam->pixel_size;
	inv = inverse_matrix(cam->transform, 4);
	if (!inv)
		inv = cam->transform;
	pixel = ft_new_point(-world_x, -world_y,-1);
	pixel_world = ft_multiply_matrix_vec(inv, pixel);
	ray.origin = ft_multiply_matrix_vec(inv, ft_new_point(0, 0, 0));
	ray.direction = vector_normilze(vector_sub(pixel_world, ray.origin));
	if (inv  != cam->transform)
		ft_free_matrix(inv , 4);
	return (ray);
}

//shadow
//to do
// iterate to each light source while i find just once source intersection i need to break;
int is_shadowed(t_world *world, t_light *light, t_point point)
{
	t_vector v;
	t_vector direction;
	float distance;
	t_ray r;
	t_intersection intersections;

	v = vector_sub(light->coordinate, point);
	direction = vector_normilze(v);
	r = create_ray(point, direction);
	distance = vec_lenght(v);
	intersections = intersect_world(world, r);
	if (intersections.n_sol > 0 && intersections.t1 >=  EPSILON && intersections.t1 < distance)
		return 1;
	return 0;
}









t_material	*get_object_material(void *obj, int obj_type)
{
	if (obj_type == SHAPE_SPHERE)
		return (((t_sphere *)obj)->material);
	else if (obj_type == SHAPE_PLANE)
		return (((t_plane *)obj)->material);
	else if (obj_type == SHAPE_CYLINDER)
		return (((t_cylinder *)obj)->material);
	else if (obj_type == SHAPE_CONE)
		return (((t_cone *)obj)->material);
	return (NULL);
}
t_color	shading_hit(t_world *world, t_compose *comp)
{
	t_color		color;
	t_material	*material;
	t_light		*current_light;

	color = ft_new_color(0, 0, 0);
	material = get_object_material(comp->obj, comp->obj_type);
	current_light = world->light;
	while (current_light)
	{
		color = ft_add_color(color, process_light(world, comp, current_light, material));
		current_light = current_light->next;
	}
	return (clamp_color(color));
}

t_color get_color_at(t_world *world, t_ray ray)
{
	t_intersection inter;
	t_compose *comp;
	t_color res;

	inter = intersect_world(world, ray);
	if (inter.n_sol <= 0)
		return ft_new_color(0, 0, 0);
	comp = prepare_computations(inter, ray);
	res = shading_hit(world, comp);
	free(comp);
	return res;
}

int key_hook(int keycode, t_scene *data)
{
	if (keycode == ESC_KEY)
	{
		ft_close_window(data);
		exit(0);
	}
	return (0);
}

int calculate_pixel_color(t_world *world, t_ray ray)
{
	t_color color;
	int     pixel_color;

	color = get_color_at(world, ray);
	pixel_color = (255 << 24) |
		(int)clamp((float)(255.999 * color.r), 0, 255) << 16 |
		(int)clamp((float)(255.999 * color.g), 0, 255) << 8 |
		(int)clamp((float)(255.999 * color.b), 0, 255);
	return (pixel_color);
}

void    render_pixel(t_scene *scene, t_world *world, t_scamera *cam, int x, int y)
{
	t_ray   ray;
	int     pixel_color;

	ray = get_ray_pixel(cam, x, y, 0.5);
	pixel_color = calculate_pixel_color(world, ray);
	my_pixel_put(&scene->data->img, x, y, pixel_color);
}

void    setup_window_hooks(t_scene *scene)
{
	mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene);
	mlx_key_hook(scene->data->win, &key_hook, scene);
	mlx_loop(scene->data->mlx);
}

int	render_image(t_scene *scene, t_world *world, t_scamera *cam)
{
	float x, y;
	int total_pixels;
	int current_pixel;
    
    total_pixels = cam->w_size * cam->h_size;
    current_pixel = 0;
    y = -1;
	mlx_clear_window(scene->data->mlx, scene->data->win);
	while(++y < cam->h_size)
	{
        x = -1;
		while (++x < cam->w_size)
		{
			render_pixel(scene, world, cam, x, y);
			current_pixel++;
		}
		display_progress(scene, cam, current_pixel, total_pixels);
	}
	mlx_clear_window(scene->data->mlx, scene->data->win);
	mlx_put_image_to_window(scene->data->mlx, scene->data->win,
						  scene->data->img.img_ptr, 0, 0);
	setup_window_hooks(scene);
	return 0;
}
