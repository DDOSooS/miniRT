#include "../../includes/minirt.h"

t_ray create_ray(t_point origin, t_vector dir)
{
    t_ray new;

    new.origin = origin;
    new.direction = dir;
    return (new);
}

t_point position(t_ray ray, float distance)
{
    t_point new;

    // Initialize all components including 'w'
    new.x = ray.origin.x + ray.direction.x * distance;
    new.y = ray.origin.y + ray.direction.y * distance;
    new.z = ray.origin.z + ray.direction.z * distance;
    new.w = 1.0;  // Points typically have w=1 in homogeneous coordinates
    return (new);
}

t_intersection ray_hit(t_intersection *inters, int count)
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
        new_shape->objects.sphere = (t_sphere *)shape_obj;
    else if (shape_type == SHAPE_PLANE)
        new_shape->objects.plane = (t_plane *)shape_obj;
    else if (shape_type == SHAPE_CYLINDER)
        new_shape->objects.cylinder = (t_cylinder *)shape_obj;
    else if (shape_type == SHAPE_CONE)
    {
        printf("cone is being created\n");
        new_shape->objects.cone = (t_cone *)shape_obj;
    }
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

t_intersection ft_intersect_cone(t_ray ray, t_cone *cone)
{
    t_intersection inter = {0, 0, NULL, 0};
    float t1, t2;

    t_vector oc = vector_sub(ray.origin, cone->apex);
    float k = cone->radius / cone->height;
    float k2 = k * k;

    float A = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z - k2 * ray.direction.y * ray.direction.y;
    float B = 2 * (ray.direction.x * oc.x + ray.direction.z * oc.z - k2 * ray.direction.y * oc.y);
    float C = oc.x * oc.x + oc.z * oc.z - k2 * oc.y * oc.y;

    float discriminant = B * B - 4 * A * C;

    if (discriminant < 0)
        return inter;

    discriminant = sqrt(discriminant);
    t1 = (-B - discriminant) / (2 * A);
    t2 = (-B + discriminant) / (2 * A);
    float y1 = ray.origin.y + t1 * ray.direction.y;
    float y2 = ray.origin.y + t2 * ray.direction.y;

    if (y1 < 0 || y1 > cone->height)
        t1 = -1;
    if (y2 < 0 || y2 > cone->height)
        t2 = -1;
    if (t1 >= 0 || t2 >= 0)
    {
        inter.n_sol = 2;
        inter.t1 = t1;
        inter.object = cone;
        inter.type = SHAPE_CONE;
    }

    return inter;
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

    //  rotation matrix && orientation vector
    float **rotation = create_rotation_matrix_from_vector(cylinder->orientation);
    // Combine rotation and transformation MX
    float **combined = ft_multiply_matrix(cylinder->transform, rotation, 4, 4);
    float **inverse = inverse_matrix(combined, 4);
    if (!inverse)
        inverse = cylinder->transform;
    // Transform ray to object space
    t_ray tr_ray = create_ray(ft_multiply_matrix_vec(inverse, ray.origin),
                                        ft_multiply_matrix_vec(inverse, ray.direction));
    float a = tr_ray.direction.x * tr_ray.direction.x + tr_ray.direction.z * tr_ray.direction.z;
    float b = 2.0f * (tr_ray.origin.x * tr_ray.direction.x + tr_ray.origin.z * tr_ray.direction.z);
    float c = tr_ray.origin.x * tr_ray.origin.x + tr_ray.origin.z * tr_ray.origin.z - cylinder->raduis * cylinder->raduis;
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
    ft_free_matrix(rotation, 4);
    ft_free_matrix(combined, 4);
    if (inverse != cylinder->transform)
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

t_vector normalize_at_cone_pos(t_cone *cone, t_vector point)
{
    t_vector apex_to_point = vector_sub(point, cone->apex);
    float height_component = vector_dot(apex_to_point, cone->axis);
    t_vector normal = vector_sub(apex_to_point, vector_mult_scalar(cone->axis, height_component));
    return vector_normilze(normal);
}

t_vector normalize_at_cylinder_pos(t_cylinder *cylinder, t_point world_p)
{
    float **rotation;
    float **combined;
    float **inv;
    t_vector world_normal;
    
    rotation = create_rotation_matrix_from_vector(cylinder->orientation);
    combined = ft_multiply_matrix(cylinder->transform, rotation, 4, 4);
    inv = inverse_matrix(combined, 4);
    if (!inv)
        inv = combined;    
    t_point local_p = ft_multiply_matrix_point(inv, world_p);
    //  normal in object space
    t_vector local_normal;
    if (fabs(local_p.y - cylinder->height/2) < EPSILON)
        local_normal = (t_vector){0, 1, 0};
    else if (fabs(local_p.y + cylinder->height/2) < EPSILON)
        local_normal = (t_vector){0, -1, 0};
    else
        local_normal = (t_vector){local_p.x, 0, local_p.z};
    // Transform normal back to world space !<= 
    ft_transpose_matrix(&inv, 4, 4);
    world_normal = ft_multiply_matrix_vec(inv, local_normal);
    ;
    ft_free_matrix(rotation, 4);
    ft_free_matrix(combined, 4);
    if (inv != combined)
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
    if (!inter)
        return (t_intersection) {0,0,NULL,0};
    i = 0;
    while (current)
    {
        if (current->type == SHAPE_SPHERE)
            inter[i] = ft_intersect_sphere(ray, current->objects.sphere);
        else if (current->type == SHAPE_PLANE)
            inter[i] = ft_intersect_plane(ray, current->objects.plane);
        else if (current->type == SHAPE_CYLINDER)
            inter[i] = ft_intersect_cylinder(ray, current->objects.cylinder);  
        else if (current->type == SHAPE_CONE)
            inter[i] = ft_intersect_cone(ray, current->objects.cone);
        current = current->next;
        i++;
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
    t_vector world_normal;
    float **inv_transpose;
    t_vector normalized;
    t_vector obj_normal;
    
    // Start with the plane's normal - no need to normalize yet
    obj_normal = plane->plane_normal;
        inv_transpose = inverse_matrix(plane->transform, 4);
    if (!inv_transpose)
        inv_transpose = plane->transform;
    ft_transpose_matrix(&inv_transpose, 4, 4);
    // Transform the normal to world space
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
    // printf("(1)=> Intersection t1: %f\n", inter.t1);
    comp->obj = inter.object;
    comp->obj_type = inter.type;
    comp->camv = negate_vector(ray.direction);
    comp->point = position(ray, comp->t);

    // printf("(2)=> comp->point: x=%f y=%f z=%f\n", comp->point.x, comp->point.y, comp->point.z);
    if (comp->obj_type == SHAPE_SPHERE)
    {
        comp->normv = normilize_at_sphere_pos((t_sphere *)(inter.object), comp->point);
        // printf("(sphere)=> Normal vector: x=%f y=%f z=%f\n", comp->normv.x, comp->normv.y, comp->normv.z);
    }
    else if (comp->obj_type == SHAPE_PLANE)
    {
        comp->normv = normalize_at_plane_pos((t_plane *)(inter.object), comp->point);
        // printf("(plane)=> Normal vector: x=%f y=%f z=%f\n",comp->normv.x, comp->normv.y, comp->normv.z);
    }
    else if (comp->obj_type == SHAPE_CYLINDER)
    {
        comp->normv = normalize_at_cylinder_pos((t_cylinder *)(inter.object), comp->point);
        // printf("(cylinder)=> Normal vector: x=%f y=%f z=%f\n", comp->normv.x, comp->normv.y, comp->normv.z);
    }
    else if (comp->obj_type == SHAPE_CONE)
    {
        comp->normv = normalize_at_cone_pos((t_cone *)(inter.object), comp->point);
        // printf("(cone)=> Normal vector: x=%f y=%f z=%f\n", comp->normv.x, comp->normv.y, comp->normv.z);
    }
    else
        comp->normv = ft_new_vector(0, 0, 0);  
    comp->normv = vector_normilze(comp->normv);
    comp->inside = 0;
    if (vector_dot(comp->normv, comp->camv) < 0.0)
    {
        comp->inside = 1;
        comp->normv = negate_vector(comp->normv);
    }
    t_point offset = ft_scale_point(comp->normv, EPSILON);
    comp->over_point = point_add(comp->point, offset);
    // printf("(4)=> over point x: %f y: %f z: %f\n", comp->over_point.x, comp->over_point.y, comp->over_point.z);
    return comp;
}

/*
    X-axis corresponds to left/right
    Y-axis corresponds to up/down
    Z-axis corresponds to forward/backward (with negative Z being forward)
*/

//camera transform matrix that transform from world coordinates to camera system Co
float **get_view_transform(t_point from_v, t_vector to_v, t_vector up_v)
{
    t_vector    forward_v;
    t_vector    left_v;
    t_vector    up_n;
    float       **view_transform;
    float       **res;
    float       **traslate_mx;

    forward_v = vector_normilze(to_v);
    up_n = vector_normilze(up_v);
    left_v = vector_cross(forward_v, up_n);
    up_v = vector_cross(left_v, forward_v);
    view_transform = identity_matrix(4);
    view_transform[0][0] = left_v.x;
    view_transform[0][1] = left_v.y;
    view_transform[0][2] = left_v.z;
    view_transform[1][0] = up_n.x;
    view_transform[1][1] = up_n.y;
    view_transform[1][2] = up_n.z;
    view_transform[2][0] = -forward_v.x;
    view_transform[2][1] = -forward_v.y;
    view_transform[2][2] = -forward_v.z;
    traslate_mx = ft_translate_matrix(ft_new_point(-from_v.x, -from_v.y, -from_v.z), 1);
    res = ft_multiply_matrix(view_transform, traslate_mx, 4, 4);
    ft_free_matrix(view_transform,4);
    ft_free_matrix(traslate_mx, 4);
    return res;
}

//y rotation than x rotation~~~~~~
float **create_rotation_matrix_from_vector(t_vector orientation)
{
    float **res;
    float sd_ax;
    t_vector normalized;
    float **rot_y;
    float **rot_x;
    
    normalized = vector_normilze(orientation);
    rot_y = rotate_y(atan2(normalized.x, normalized.z));
    rot_x = rotate_x(asin(-normalized.y));
    res = ft_multiply_matrix(rot_y, rot_x, 4, 4);
    ft_free_matrix(rot_y, 4);
    ft_free_matrix(rot_x, 4);
    return res;
}

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

void ft_add_cone_shape(t_world *world, t_cone *cone)
{
    t_cone *tmp;

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

t_world *default_world(t_scene *scene)
{
    t_world *world = malloc(sizeof(t_world));
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
    return world;
}



void ft_set_camera(s_camera **camera)
{
    float aspect;
    float half_view;
   
    aspect = (*camera)->w_size / (*camera)->h_size;
    half_view = tan((*camera)->fov / 2.0f);
    if (aspect >= 1.0f)
    {
        (*camera)->half_w_size = half_view;
        (*camera)->half_h_size = half_view / aspect;
    }
    else
    {
        (*camera)->half_w_size = half_view * aspect;
        (*camera)->half_h_size = half_view;
    }
    (*camera)->pixel_size = (*camera)->half_w_size * 2.0f / (*camera)->w_size;
    (*camera)->transform = get_view_transform((*camera)->origin,
        vector_normilze((*camera)->direction), ft_new_vector(0, 1, 0));
}

t_ray get_ray_pixel(s_camera *cam, float x, float y, float edge)
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

t_color get_checkered_color(t_sphere *sphere, float u, float v)
{
    int scale = 20;
    int u_scaled = (int)(u * scale);
    int v_scaled = (int)(v * scale);

    if ((u_scaled + v_scaled) % 2 == 0)
        return sphere->sphere_color;
    return sphere->checkered_color;
}

void get_spherical_coordinates(t_vector hit_point, t_sphere *sphere, float *u, float *v)
{
    t_vector local_point = vector_sub(hit_point, sphere->sphere_coordinates);
    local_point = vector_normilze(local_point);

    // printf("local_point: x = %f, y = %f, z = %f\n", local_point.x, local_point.y, local_point.z);
    float theta = atan2(local_point.z, local_point.x);
    float phi = acos(local_point.y);

    *u = (theta + M_PI) / (2 * M_PI);
    *v = phi / M_PI;
    // printf("UV coordinates: u = %f, v = %f\n", *u, *v);
}
t_color int_to_color(int color)
{
    t_color result;
    result.r = (color >> 16) & 0xFF; // Extract red component
    result.g = (color >> 8) & 0xFF;  // Extract green component
    result.b = color & 0xFF;         // Extract blue component
    return result;
}

t_color sample_texture(t_texture *texture, float u, float v)
{
    u = fmod(u, 1.0f);
    v = fmod(v, 1.0f);
    if (u < 0) u += 1.0f;
    if (v < 0) v += 1.0f;
    int x = (int)(u * texture->width);
    int y = (int)(v * texture->height);
    x = x % texture->width;
    y = y % texture->height;
    int pixel_index = y * texture->size_line + x * (texture->bpp / 8);
    int color = *(int *)(texture->img_data + pixel_index);

    t_color result = int_to_color(color);
    // printf("Sampled color: r = %f, g = %f, b = %f\n", result.r, result.g, result.b);
    return result;
}

t_color get_textured_lighting_color(t_color texture_color, t_material *material, t_light *light, t_compose *comp, int shadow)
{
    t_vector light_dir_normal;
    t_color eff_color, ambient, diffuse, specular;
    float light_dot_normal, reflect_dot_camera;

    // printf("Texture color: r = %f, g = %f, b = %f\n", texture_color.r, texture_color.g, texture_color.b);
    eff_color = ft_multiply_color(texture_color, light->color);
    ambient = clamp_color(ft_multiply_color_scalar(eff_color, material->ambient));  
    // printf("Ambient color: r = %f, g = %f, b = %f\n", ambient.r, ambient.g, ambient.b);
    if (shadow)
        return ambient;
    light_dir_normal = vector_normilze(vector_sub(light->coordinate, comp->point));
    light_dot_normal = vector_dot(light_dir_normal, comp->normv);
    if (light_dot_normal < EPSILON)
        return ambient;
    else
    {
        diffuse = ft_multiply_color_scalar(eff_color, material->diffuse * light_dot_normal);
        reflect_dot_camera = vector_dot(reflect_vector(negate_vector(light_dir_normal),
                                        comp->normv), comp->camv);
        // printf("reflect_dot_camera: %f\n", reflect_dot_camera);
        // fflush(stdout);
        // printf("material->shininess: %f\n", material->shininess);
        // fflush(stdout);
        if (reflect_dot_camera <= EPSILON)
            specular = ft_new_color(0, 0, 0); 
        else
            specular = ft_multiply_color_scalar(light->color,
                        material->specular * powf(reflect_dot_camera, material->shininess));
    }
    // printf("Diffuse color: r = %f, g = %f, b = %f\n", diffuse.r, diffuse.g, diffuse.b);
    // printf("Specular color: r = %f, g = %f, b = %f\n", specular.r, specular.g, specular.b);
    t_color tmp = ft_add_color(specular, diffuse);
    t_color final_color = clamp_color(ft_add_color(tmp, ambient));
    // printf("Final color: r = %f, g = %f, b = %f\n", final_color.r, final_color.g, final_color.b);
    return final_color;
}

t_color shading_hit(t_world *world, t_compose *comp)
{
    t_color color = ft_new_color(0, 0, 0);
    t_material *material = NULL;
    int shadowed = 0;
    t_light *tmp_light;
    int i;

    i = -1;
    tmp_light = world->light;
    while (tmp_light)
    {
        if (comp->obj_type == SHAPE_SPHERE)
        {
            material = ((t_sphere *)comp->obj)->material;
            t_color base_color = material->color; // Default base color

            if (((t_sphere *)comp->obj)->has_checkered)
            {
                float u = 0, v = 0;
                get_spherical_coordinates(comp->over_point, (t_sphere *)comp->obj, &u, &v);
                base_color = get_checkered_color((t_sphere *)comp->obj, u, v); // Use checkered color as base
            }
            else if (((t_sphere *)comp->obj)->has_texture)
            {
                float u = 0, v = 0;
                get_spherical_coordinates(comp->over_point, (t_sphere *)comp->obj, &u, &v);
                color = sample_texture(((t_sphere *)comp->obj)->texture, u, v);
                color.r /= 255.0f;
                color.g /= 255.0f;
                color.b /= 255.0f;
                shadowed = is_shadowed(world,tmp_light, comp->over_point);
                color = ft_add_color(color, get_textured_lighting_color(color, material, tmp_light, comp, shadowed));
            }

            shadowed = is_shadowed(world, tmp_light, comp->over_point);
            color = ft_add_color(color, get_lighting_color(material, tmp_light, comp, shadowed, base_color));
        }
        else if (comp->obj_type == SHAPE_PLANE) {
            material = ((t_plane *)comp->obj)->material;
        }
        else if (comp->obj_type == SHAPE_CYLINDER)
            material = ((t_cylinder *)comp->obj)->material;
        else if (comp->obj_type == SHAPE_CONE)
            material = ((t_cone *)comp->obj)->material;
        shadowed = is_shadowed(world, tmp_light, comp->over_point);
        color = ft_add_color(color, get_lighting_color(material, tmp_light, comp, shadowed, material->color));
        tmp_light = tmp_light->next;
    }
    return clamp_color(color);
}

t_color get_color_at(t_world *world, t_ray ray)
{
    t_intersection inter;
    t_compose *comp;
    t_color res;

    inter = intersect_world(world, ray);
    if (inter.n_sol <= 0)
    {
        // printf("no intersection\n");
        return ft_new_color(0, 0, 0);
    }
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

char *int_to_str(int num) 
{
    int len = 0;
    int temp = num;
    char *str;

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
        return NULL;
    str[len] = '\0';
    for (int i = len - 1; i >= 0; i--)
    {
        str[i] = (num % 10) + '0';
        num /= 10;
    }
    return str;
}

void ft_strcpy(char *dest, const char *src)
{
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
}

void ft_strcat(char *dest, const char *src)
{
    while (*dest)
        dest++;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
}


int render_image(t_scene *scene, t_world *world, s_camera *cam)
{
    float x, y;
    t_color color;
    t_ray ray;
    int pixel_color;
    int total_pixels = cam->w_size * cam->h_size;
    int current_pixel = 0;

    mlx_clear_window(scene->data->mlx, scene->data->win);

    for (y = 0; y < cam->h_size; y++)
    {
        for (x = 0; x < cam->w_size; x++)
        {
            ray = get_ray_pixel(cam, x, y, 0.5);
            // printf("ray origin: x = %f, y = %f, z = %f\n", ray.origin.x, ray.origin.y, ray.origin.z);
            // printf("ray direction: x = %f, y = %f, z = %f\n", ray.direction.x, ray.direction.y, ray.direction.z);
            color = get_color_at(world, ray);
            pixel_color = (255 << 24) |
                (int)clamp((float)(255.999 * color.r), 0, 255) << 16 |
                (int)clamp((float)(255.999 * color.g), 0, 255) << 8 |
                (int)clamp((float)(255.999 * color.b), 0, 255);
            my_pixel_put(&scene->data->img, x, y, pixel_color);
            current_pixel++;
        }

        int progress = (int)((float)current_pixel / total_pixels * 100);
        char *progress_str = malloc(20);
        if (!progress_str)
            continue; 
        char *progress_num = int_to_str(progress);
        if (!progress_num)
        {
            free(progress_str);
            continue;
        }
        ft_strcpy(progress_str, "Loading... ");
        ft_strcat(progress_str, progress_num);
        ft_strcat(progress_str, "%");
        mlx_clear_window(scene->data->mlx, scene->data->win);
        mlx_string_put(scene->data->mlx, scene->data->win, 
                       cam->w_size / 2 - 50, cam->h_size / 2, 0xFFFFFF, progress_str);
        free(progress_str);
        free(progress_num);
    }
    mlx_clear_window(scene->data->mlx, scene->data->win);
    mlx_put_image_to_window(scene->data->mlx, scene->data->win,
                            scene->data->img.img_ptr, 0, 0);
    mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene);
    mlx_key_hook(scene->data->win, &key_hook, scene);
    mlx_loop(scene->data->mlx);
    return 0;
}
