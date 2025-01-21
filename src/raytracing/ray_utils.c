#include "../../includes/minirt.h"

t_ray *create_ray(t_point *origin, t_vector *dir)
{
    t_ray *new;

    new = malloc(sizeof(t_ray));
    if (!new)
        return (NULL);
    new->origin = origin;
    new->direction = dir;
    return (new);
}

t_point *position(t_ray *ray, float distance)
{
    t_point *new;

    new = ft_new_point(0,0,0);
    if (!new)
        return (NULL);
    new->x = ray->origin->x + ray->direction->x * distance;
    new->y = ray->origin->y + ray->direction->y * distance;
    new->z = ray->origin->z + ray->direction->z * distance;
    return (new);
}


t_intersection *ray_hit(t_intersection *inters, int count)
{
    float min_t = INFINITY;
    t_intersection *result = &inters[0];
    int i = 0;
    
    while (i < count)
    {
        if (inters[i].t1 > 0 && inters[i].t1 < min_t)
        {
            min_t = inters[i].t1;
            result = &inters[i];
        }
        i++;
    }
    return result;
}

t_intersection ft_intersect_sphere(t_ray *ray, t_sphere *sphere)
{
    t_intersection result;
    t_vector *sphere_to_ray;
    float a, b, c, discriminant;
    
    sphere_to_ray = vector_sub(ray->origin, sphere->sphere_coordinates);
    
    a = vector_dot(ray->direction, ray->direction);
    b = 2 * vector_dot(ray->direction, sphere_to_ray);
    c = vector_dot(sphere_to_ray, sphere_to_ray) - (sphere->sphere_diameter * sphere->sphere_diameter);
    
    discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0)
    {
        result.t1 = -1;
        result.t2 = -1;
        result.n_sol = 0;
    }
    else
    {
        result.t1 = (-b - sqrt(discriminant)) / (2 * a);
        result.t2 = (-b + sqrt(discriminant)) / (2 * a);
        if (result.t1 > 0 && result.t2 > 0 && result.t1 > result.t2)
            result.t1 = result.t2;
        result.n_sol = 2;
        result.object = sphere;
        result.type = SHAPE_SPHERE;
    }
    return result;
}
t_intersection  ft_new_intersection(float t, void *object, int type)
{
    t_intersection intersection;

    intersection.t1 = t;
    intersection.t2 = t;
    intersection.object = object;
    intersection.type = type;
    return (intersection);
}

t_ray *transform(t_ray *ray, float **m)
{
    t_vector *dir;
    t_point *origin;

    if (!m)
        printf("===> NULLLLLLLLLLLL  <===\n");
    dir = ft_multiply_matrix_vec(m, ray->direction);
    origin = ft_multiply_matrix_vec(m, ray->origin);
    return create_ray(origin, dir);
}


t_vector *normilize_at_sphere_pos(t_sphere *sphere, t_point *w_p)
{
    t_vector *vec;
    t_vector *obj_n;
    t_point *obj_p;
    t_vector *tmp;
    float **inv_m;

    inv_m = inverse_matrix(sphere->transform, 4);
    if (!inv_m)
        inv_m = sphere->transform;
    obj_p = ft_multiply_matrix_vec(inv_m, w_p);
    obj_n = vector_sub(w_p, sphere->sphere_coordinates);
    ft_transpose_matrix(&inv_m, 4,4);
    tmp = ft_multiply_matrix_vec(inv_m, obj_n);
    tmp->w = 0;
    vec = vector_normilze(tmp);
    free(tmp);
    free(obj_n);
    free(obj_p);
    ft_free_matrix(inv_m, 4);
    return vec;
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
    // sphere->material->color = ft_new_color(0.8, 1.0, 0.6);
    // sphere->material->diffuse = 0.7;
    // sphere->material->specular = 0.2;
    sphere->next = NULL;
    return sphere;
}

p_light *default_light() {
    p_light *light;

    light = malloc(sizeof(p_light));
    if (!light)
        return (NULL);
    light->position = ft_new_point(-10, 10, -10);
    light->intensity = ft_new_color(1, 1, 1);
    return light;
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

t_intersection ft_intersect_plane(t_ray *ray, t_plane *plane)
{
    t_intersection result = {0, 0, 0, NULL, 0};
    
    // 1. Calculate denominator (dot product of ray direction and plane normal)
    float denom = vector_dot(ray->direction, plane->plane_normal);
    
    // 2. Check for parallel ray (denominator near zero)
    if (fabs(denom) < EPSILON)  
    {
        return result;  // Ray is parallel to plane, no intersection
    }
    
    // 3. Calculate vector from ray origin to a point on the plane
    t_vector *origin_to_plane = vector_sub(ray->origin, plane->plane_cordinates);  // Fixed: use plane->point instead of plane->plane_normal
    
    // 4. Calculate intersection distance
    float t = -(vector_dot(origin_to_plane, plane->plane_normal)) / denom;
    
    // 5. Free temporary vector
    free(origin_to_plane);
    
    // 6. Check if intersection is behind ray
    if (t < 0)
    {
        return result;  // Intersection is behind ray origin
    }
    
    // 7. Set intersection details
    result.t1 = t;
    result.object = plane;
    result.type = SHAPE_PLANE;
    
    return result;
}

t_intersection *intersect_world(t_world *world, t_ray *ray)
{
    t_intersection *res;
    int i;

    t_shape *current = world->shape;
    t_intersection *inter = malloc(sizeof(t_intersection) * world->n_objects);
    if (!inter)
        return NULL;
    i = 0;
    while (current)
    {
        if (current->type == SHAPE_SPHERE)  // Only consider spheres for now.
            inter[i] = ft_intersect_sphere(ray, current->objects.sphere);
        else if (current->type == SHAPE_PLANE)
            inter[i] = ft_intersect_plane(ray, current->objects.plane);
        // else if (current->type == SHAPE_CYLINDER)

        current = current->next;
        i++;
    }
    res = ray_hit(inter, world->n_objects);
    return res;
}

t_vector *normalize_at_plane_pos(t_plane *plane, t_point *w_p)
{
    t_vector *vec, *tmp;
    t_vector *obj_n;
    float **inv_m;
    
    obj_n = vector_normilze(plane->plane_normal); 
    inv_m = inverse_matrix(plane->transform, 4);
    if (!inv_m) {
        inv_m = plane->transform; 
    }
    
    
    ft_transpose_matrix(&inv_m, 4, 4);
    tmp = ft_multiply_matrix_vec(inv_m, obj_n);
    tmp->w = 0; 
    
    vec = vector_normilze(tmp);
    
    free(tmp);
    free(obj_n);
    if (inv_m != plane->transform)
    {
        ft_free_matrix(inv_m, 4);
    }
    return vec;
}


t_compose *prepare_computations(t_intersection *inter, t_ray *ray)
{
    t_compose *comp = malloc(sizeof(t_compose));

    comp->t = inter->t1;
    comp->obj = inter->object;
    comp->obj_type = inter->type;
    comp->camv = negate_vector(ray->direction);
    comp->point = position(ray, comp->t);

    // Determine the normal at the intersection point
    if (comp->obj_type == SHAPE_SPHERE)
        comp->normv = normilize_at_sphere_pos((t_sphere *)(inter->object), comp->point);
    else if (comp->obj_type == SHAPE_PLANE)
        comp->normv = normalize_at_plane_pos((t_plane *)(inter->object), comp->point);

    comp->normv = vector_normilze(comp->normv);

    if (vector_dot(comp->normv, comp->camv) < 0.0)
    {
        comp->inside = 1;
        comp->normv = negate_vector(comp->normv);
    }
    else
    {
        comp->inside = 0;
    }

    // comp->over_point = vector_add(comp->point, vector_multiply_scalar(comp->normv, EPSILON));
    comp->over_point = vector_add(comp->point, comp->normv);

    return comp;
}



float **get_view_transform(t_point *from_v, t_point *to_v, t_vector *up_v)
{
    float **view_transform;
    t_vector *forward_v;
    t_vector *left_v;
    t_vector *up_n;
    float  **res;

    forward_v = vector_sub(to_v, from_v);
    forward_v = vector_normilze(forward_v);

    up_n = vector_normilze(up_v);
    left_v = vector_cross(forward_v, up_n);
    // side_v = vector_normilze(side_v);
    up_v = vector_cross(left_v, forward_v);

    view_transform = identity_matrix(4);
    view_transform[0][0] = left_v->x;
    view_transform[0][1] = left_v->y;
    view_transform[0][2] = left_v->z;
    view_transform[1][0] = up_n->x;
    view_transform[1][1] = up_n->y;
    view_transform[1][2] = up_n->z;
    view_transform[2][0] = -forward_v->x;
    view_transform[2][1] = -forward_v->y;
    view_transform[2][2] = -forward_v->z;
    res= ft_multiply_matrix(view_transform, ft_translate_matrix(ft_new_point(-from_v->x, -from_v->y, -from_v->z),1),4,4);
    return res;
}

t_world *default_world()
{
    t_world *world = malloc(sizeof(t_world));
    if (!world)
        return NULL;
    world->n_objects = 0;
    world->shape = NULL;

    t_sphere *sphere1 = default_sphere();
    sphere1->sphere_diameter = 1.5;
    sphere1->sphere_coordinates = ft_new_point(0,0,0);
    sphere1->transform = ft_translate_matrix(ft_new_point(-1, 0, 0), 1);
    sphere1->material->color = ft_new_color(1, 0.2, 1);
    sphere1->material->diffuse = 0.7;
    sphere1->material->specular = 0.3;

    // sphere2->sphere_diameter = 1.5;
    // sphere2->sphere_coordinates = ft_new_point(3,0,0);
    // sphere2->transform = ft_translate_matrix(ft_new_point(2, 0, 0), 1);
    // sphere2->material->color = ft_new_color(0.2, 1, 0.2);
    // sphere2->material->diffuse = 0.7;
    // sphere2->material->specular = 0.3;

        t_plane *plane = malloc(sizeof(t_plane));
        // Create a gentle slope
        plane->plane_normal = ft_new_vector(1, 1, 0);
        // Normalize the vector (very important!)
        // plane->plane_normal = vector_normilze(plane->plane_normal);
        plane->plane_cordinates = ft_new_point(0, -1, 0);
        plane->plane_color = ft_new_color(0.8, 0.8, 0.8);
        plane->transform = identity_matrix(4);
        plane->material = default_material();
        plane->material->color = ft_new_color(0.2, 1.2, 0.2);
        plane->material->diffuse = 0.7;
        plane->material->specular = 0.3;

    t_plane *plane1 = malloc(sizeof(t_plane));
    plane1->plane_normal = ft_new_vector(0, 0, 1);  // Facing towards camera
    plane1->plane_cordinates = ft_new_point(0, 0, -10);
    plane1->plane_color = ft_new_color(0.8, 1, 0.8);
    plane1->transform = identity_matrix(4);
    plane1->material = default_material();
    plane1->material->color = ft_new_color(1, 1.2, 0.2);
    // plane1->material->diffuse = 0.9;
    // plane1->material->specular = 0.9;

    // Light source
    world->light = ft_new_plight(ft_new_color(1, 1, 1), ft_new_point(100, 50,0));

    // Add objects to world
    ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    ft_add_shape(&world, plane, SHAPE_PLANE);
    // ft_add_shape(&world, plane1, SHAPE_PLANE);

    return world;
}
s_camera  *new_camera(float hsize, float vsize, float fov)
{
    s_camera *camera;
    float    aspect;
    float    half_view;

    camera = malloc(sizeof(s_camera));
    if (!camera)
        return NULL;
    camera->h_size = hsize;
    camera->w_size = vsize;
    camera->fov = fov;
    camera->transform = identity_matrix(4);
    aspect = camera->h_size / camera->w_size;
    half_view = tan(camera->fov / 2.0f);
    if (aspect >= 1)
    {
        camera->half_h_size = half_view * aspect;
        camera->half_w_size = half_view;
    }
    else
    {
        camera->half_h_size = half_view;
        camera->half_w_size = half_view / aspect;
    }
    camera->pixel_size = (camera->half_w_size * 2.0f) / camera->h_size;
    return camera;
}

t_ray *get_ray_pixel(s_camera *cam, float x, float y)
{
    t_ray *ray;
    float x_off;
    float y_off;
    float w_x;
    float w_y;
    t_point *pixel;

    ray = malloc(sizeof(t_ray));
    x_off = (x + 0.5) * cam->pixel_size;
    y_off = (y + 0.5) * cam->pixel_size;
    w_x = cam->half_w_size *cam->pixel_size;
    w_y = cam->half_h_size * cam->pixel_size;
    pixel = ft_multiply_matrix_vec(inverse_matrix(cam->transform, 4), ft_new_point(w_x, w_y, -1)); 
    ray->origin = ft_multiply_matrix_vec( inverse_matrix(cam->transform,4) , ft_new_point(0,0,0));
    ray->direction = vector_normilze( vector_sub(pixel, ray->origin));
    return (ray);
}

//shadow 

int is_shadowed(t_world *world, t_point *point)
{
    t_vector *v = vector_sub(world->light->position, point);
    float distance = vec_lenght(v);
    t_vector *direction = vector_normilze(v);
    
    t_ray *r = create_ray(point, direction);
    t_intersection *intersections = intersect_world(world, r);
    
    if (intersections && intersections->t1 > 0 && intersections->t1 < distance)
    {
        free(v);
        free(direction);
        free(r);
        return 1;
    }
    free(v);
    free(direction);
    free(r);
    return 0;
}


t_color *shading_hit(t_world *world, t_compose *comp)
{
    t_color *color;
    t_material *material;
    

    if (comp->obj_type == SHAPE_SPHERE)
        material = ((t_sphere *)comp->obj)->material;
    if (comp->obj_type == SHAPE_PLANE)
        material = ((t_plane *)comp->obj)->material;
    
    // Check if point is in shadow before calculating full lighting ??
    if (is_shadowed(world, comp->point))
    {
        //calculating  ambient light
        t_color *ambient = ft_multiply_color_scalar(
            ft_multiply_color(material->color, world->light->intensity),
            material->ambient
        );
        printf("hehoo\n");
        return clamp_color(ambient);
    }
    
    color = get_lighting_color(material, world->light, comp->point,
                             comp->camv, comp->normv);
    return color;
}

t_color *get_color_at(t_world *world, t_ray *ray)
{
    t_intersection *inter;
    t_compose *comp;
    t_color *res;

    inter = intersect_world(world, ray);
    if (!inter)
        return ft_new_color(0, 0, 0);

    comp = prepare_computations(inter, ray);
    res = shading_hit(world, comp);

    free(comp);
    free(inter);
    return res;
}


int render_image(t_scene *scene, t_world *world, s_camera *cam)
{
    int x, y;
    t_color *color;
    t_ray *ray;
    int pixel_color;

    for (y = 0; y < cam->w_size; y++)
    {
        for (x = 0; x < cam->h_size; x++)
        {
            ray = get_ray_pixel(cam, x, y);
            color = get_color_at(world, ray);
            pixel_color = (255 << 24) | 
                         ((int)(255 * color->r) << 16) | 
                         ((int)(255 * color->g) << 8) | 
                         ((int)(255 * color->b) << 0);

            my_pixel_put(&scene->data->img, x, y, pixel_color);
            free(ray);
            free(color);
        }
    }
    printf("Putting image to window...\n");
    mlx_put_image_to_window(scene->data->mlx, scene->data->win, 
                            scene->data->img.img_ptr, 0, 0);
    mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene->data);
    mlx_loop(scene->data->mlx);
    return 0;
}

// t_vector *get_normat_at(t_shape *shape, t_point *point)
// {
//     t_vector *norm;

//     switch (shape->type)
//     {
//         case SHAPE_SPHERE:
//             norm = normilize_at_sphere_pos(shape->objects.sphere, point);
//             break;
//         // case SHAPE_PLANE:
//         //     norm = normilize_at_plan_pos(shape->objects.sphere, point);
//         //     break;
//         default:
//             printf("Unknown shape type\n");
//             return NULL;
//     }
//     return norm;
// }

// t_intersection *intersection(t_ray *ray, t_shape *shape)
// {
//     t_ray *tmp;

//     tmp = transform(ray, shape->objects.sphere->transform);
//     return get_local_intersection(tmp, shape);
// }