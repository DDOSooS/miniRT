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

// t_intersection *ray_hit(t_intersection *inters, int count)
// {
//     t_intersection * hit;
//     float min_t = INT_MAX;
//     int i;

//     hit = NULL;
//     i = -1;
//     while (++i < count)
//     {
//         if (inters[i].t1 >= 0 && inters[i].t1 < min_t)
//         {
//             min_t = inters[i].t1;
//             hit = &inters[i];
//         }
//     }
//     return (hit);
// }

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
    sphere->sphere_diameter = 1;
    sphere->transform = identity_matrix(4);
    sphere->material = default_material();
    // sphere->material->color = ft_new_color(0.8, 1.0, 0.6);
    // sphere->material->diffuse = 0.7;
    // sphere->material->specular = 0.2;
    sphere->next = NULL;
    return sphere;
}

p_light *default_light()
{
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
        inter[i] = ft_intersect_sphere(ray, current->objects.sphere);
        current = current->next;
        i++;
    }
    res = ray_hit(inter, world->n_objects);
    return res;
}

t_compose *prepare_computations(t_intersection *inter, t_ray *ray)
{
    t_compose *comp;

    comp = malloc(sizeof(t_compose));

    comp->t = inter->t1;
    comp->obj = inter->object;
    comp->obj_type = inter->type;
    comp->camv = negate_vector(ray->direction);
    comp->point = position(ray, comp->t);
    comp->normv = normilize_at_sphere_pos((t_sphere *)(inter->object),comp->point);
    if (vector_dot(comp->normv, comp->camv) < 0.0)
    {
        comp->inside = 1;
        comp->normv = negate_vector(comp->normv);
    }
    else
        comp->inside = 0;
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

    // Create first sphere
    t_sphere *sphere1 = malloc(sizeof(t_sphere));
    if (!sphere1)
    {
        free(world);
        return NULL;
    }
    sphere1->sphere_coordinates = ft_new_point(-1.5, 0, 0);
    sphere1->sphere_diameter = 1;
    sphere1->transform = identity_matrix(4);
    sphere1->material = default_material();
    sphere1->material->color = ft_new_color(1, 0.2, 1);  // Purple
    sphere1->material->ambient = 0.1;
    sphere1->material->diffuse = 0.9;
    sphere1->material->specular = 0.9;
    sphere1->material->shininess = 200.0;

    // Create second sphere
    t_sphere *sphere2 = malloc(sizeof(t_sphere));
    if (!sphere2)
    {
        free(sphere1->material);
        free(sphere1);
        free(world);
        return NULL;
    }
    sphere2->sphere_coordinates = ft_new_point(1.5, 0, 0);
    sphere2->sphere_diameter = 1;
    sphere2->transform = identity_matrix(4);
    sphere2->material = default_material();
    sphere2->material->color = ft_new_color(0.2, 1, 0.2);  // Green
    sphere2->material->ambient = 0.1;
    sphere2->material->diffuse = 0.7;
    sphere2->material->specular = 0.9;
    sphere2->material->shininess = 150.0;

    ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    ft_add_shape(&world, sphere2, SHAPE_SPHERE);

    t_point *light_position = ft_new_point(-10, 10, -10);
    world->light = ft_new_plight(ft_new_color(1, 1, 1), light_position);

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
    printf("Ray Origin: (%.2f, %.2f, %.2f), Direction: (%.2f, %.2f, %.2f)\n",
        ray->origin->x, ray->origin->y, ray->origin->z,
        ray->direction->x, ray->direction->y, ray->direction->z);
    return (ray);
}
inline t_color *shading_hit(t_world *world, t_compose *comp) {
    t_color *color;
    
    color = get_lighting_color(((t_sphere *)(comp->obj))->material, world->light, comp->point, comp->camv, comp->normv);
    return color;
}

t_color *get_color_at(t_world *world, t_ray *ray)
{
    t_intersection *inter;
    t_compose *comp;
    t_color *res;

    inter = intersect_world(world, ray);
    if (!inter)
    {
        printf("No intersection found\n");
        return ft_new_color(0, 0, 0);
    }
    else
        printf("intersection found\n");
    comp = prepare_computations(inter, ray);
    res = shading_hit(world, comp);

    free(comp);
    free(inter);
    return res;
}


int render_image(t_scene *scene, t_world *t_world, s_camera *cam)
{
    int x;
    int y;
    t_color *color;
    t_ray *ray;
    int pixel_color;
    // int pixels_drawn = 0;  // Debug counter

    printf("Starting render with dimensions: %f x %f\n", cam->h_size, cam->w_size);

    for (y = 0; y < cam->w_size; y++)
    {
        for (x = 0; x < cam->h_size; x++)
        {
            ray = get_ray_pixel(cam, x, y);
            color = get_color_at(t_world, ray);
             printf("Pixel (%d, %d) Color: R: %.2f, G: %.2f, B: %.2f\n", x, y, color->r, color->g, color->b);

            pixel_color = (255 << 24) | 
                         ((int)(255 * color->r) << 16) | 
                         ((int)(255 * color->g) << 8) | 
                         ((int)(255 * color->b) << 0);
            // printf("pixel_color = %f - %f - %f\n", color->r, color->g, color->b);
            my_pixel_put(&scene->data->img, x, y, pixel_color);
            // pixels_drawn++;

            // Free resources for this pixel
            free(ray);
            free(color);
        }
    }

    // printf("Finished rendering. Drew %d pixels\n", pixels_drawn);
    printf("Putting image to window...\n");

    mlx_put_image_to_window(scene->data->mlx, scene->data->win, 
                           scene->data->img.img_ptr, 0, 0);
    mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene->data);
    mlx_loop(scene->data->mlx);
    return 0;
}


// int render_spheres(t_scene *scene)
// {
//     // Wall and pixel calculations
//     float wall_size = 50;
//     float pixel_size = wall_size / scene->image_width;
//     float half_size = wall_size / 2.0;
//     float wall_z = 10.0;

//     // Create first sphere
//     t_world *world = default_world();

//     // Setup light

//     t_point *ray_origin = ft_new_point(0, 0, -5);

//     for (int y = 0; y < scene->image_height; y++)
//     {
//         float world_y = half_size - pixel_size * y;
        
//         for (int x = 0; x < scene->image_width; x++)
//         {
//             float world_x = -half_size + pixel_size * x;

//             t_point *wall_point = ft_new_point(world_x, world_y, wall_z);
//             t_vector *ray_direction = vector_sub(wall_point, ray_origin);
//             t_ray *ray = create_ray(ray_origin, vector_normilze(ray_direction));


//             // Determine which sphere is closer (if any intersection)
//             float t = -1;
//             t_intersection *inter = intersect_world(world, ray);
//             t_sphere *hit_sphere = inter->object;
//             if (inter->t1 > 0)
//                 t = inter->t1;

//             if (hit_sphere)
//             {
//                 t_point *hit_point = position(ray, t);
//                 t_vector *normal = normilize_at_sphere_pos(hit_sphere, hit_point);
//                 t_vector *cam_v = negate_vector(vector_normilze(ray->direction));
                
//                 t_color *color = get_lighting_color(hit_sphere->material, world->light, hit_point, cam_v, normal);

//                 int color_value = (255 << 24) | 
//                                 ((int)(255.0 * color->r) << 16) | 
//                                 ((int)(255.0 * color->g) << 8) | 
//                                 ((int)(255.0 * color->b));
//                 my_pixel_put(&scene->data->img, x, y, color_value);

//                 free(hit_point);
//                 free(normal);
//                 free(cam_v);
//                 free(color);
//             }
//             free(wall_point);
//             free(ray_direction);
//             free(ray);
//         }
//     }
//     printf("end of render\n");
//     // Display the result
//     mlx_put_image_to_window(scene->data->mlx, scene->data->win, scene->data->img.img_ptr, 0, 0);
//     mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene->data);
//     mlx_loop(scene->data->mlx);

//     // Free all resources

//     free(ray_origin);

//     return (1);
// }