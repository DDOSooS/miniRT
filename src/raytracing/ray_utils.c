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

t_intersection ft_intersect_sphere(t_ray *ray, t_sphere *sphere)
{
    t_intersection intersection;
    t_vector *oc;
    t_ray *tmp;
    float a, b, c, discriminant;
    float **t;

    intersection.object = sphere;
    intersection.type = 1;
    intersection.n_sol = 0;
    intersection.t1 = 0;
    intersection.t2 = 0;
    t = inverse_matrix(sphere->transform, 4);
    if (!t)
        return intersection;  
    tmp = transform(ray, t);
    oc = vector_sub(tmp->origin, sphere->sphere_coordinates);
    a = vector_dot(tmp->direction, tmp->direction);
    b = 2 * vector_dot(tmp->direction, oc);
    c = vector_dot(oc, oc) - sphere->sphere_diameter * sphere->sphere_diameter;
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return intersection;
    if (discriminant == 0)
    {
        intersection.n_sol = 1;
        intersection.t1 = -b / (2 * a);
        return intersection;
    }
    intersection.n_sol = 2;
    float sqrt_disc = sqrt(discriminant);
    float t1 = (-b - sqrt_disc) / (2 * a);
    float t2 = (-b + sqrt_disc) / (2 * a);
    intersection.t1 = fmin(t1, t2);
    intersection.t2 = fmax(t1, t2);
    return intersection;
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

t_intersection *ray_hit(t_intersection *inters, int count)
{
    t_intersection * hit;
    float min_t = INT_MAX;
    int i;

    hit = NULL;
    i = -1;
    while (++i < count)
    {
        if (inters[i].t1 >= 0 && inters[i].t1 < min_t)
        {
            min_t = inters[i].t1;
            hit = &inters[i];
        }
    }
    return (hit);
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
t_world *default_world()
{
    t_world *world = malloc(sizeof(t_world));
    if (!world)
        return NULL;
    world->n_objects = 0;
    world->shape = NULL;
    
    /*
    // Create first sphere with specific properties
    t_sphere *sphere1 = default_sphere();
    sphere1->material->color = ft_new_color(0.8, 1.0, 0.6);
    sphere1->material->ambient = 0.1;
    sphere1->material->diffuse = 0.7;
    sphere1->material->specular = 0.2;
    
    // Create second sphere with different properties
    t_sphere *sphere2 = default_sphere();
    sphere2->material->color = ft_new_color(1.0, 1.0, 1.0);  // White color
    sphere2->material->ambient = 0.1;
    sphere2->material->diffuse = 0.9;  // Higher diffuse
    sphere2->material->specular = 0.9; // Higher specular
    sphere2->transform = ft_scaling_matrix(0.5, 0.5, 0.5, 1);
    
    p_light *light1 = default_light();
    world->shape = NULL;
    world->light = light1;

    ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    ft_add_shape(&world, sphere2, SHAPE_SPHERE);
    printf("world shape n_object: %d\n", world->n_objects);
    */
    t_sphere *floor = default_sphere();
    floor->transform = ft_scaling_matrix(10,0.01,10,1);
    floor->material->color = ft_new_color(1,0.9,0.9);
    floor->material->specular = 0;

    t_sphere *left_W = default_sphere();
    left_W->transform = ft_multiply_matrix(ft_translate_matrix(ft_new_point(0,0,5),1), 
                        ft_multiply_matrix(rotate_y(-PI/4),
                        ft_multiply_matrix(rotate_x(PI / 2), ft_scaling_matrix(10,0.01,10,1),4,4),4,4),4,4);
    left_W->material = floor->material;

    
    t_sphere *right_W = default_sphere();
    right_W->transform = ft_multiply_matrix(ft_translate_matrix(ft_new_point(0,0,5),1), 
                        ft_multiply_matrix(rotate_y(PI/4),
                        ft_multiply_matrix(rotate_x(PI / 2), ft_scaling_matrix(10,0.01,10,1),4,4),4,4),4,4);
    right_W->material = floor->material;

    t_sphere *middle = default_sphere();
    middle->transform = ft_translate_matrix(ft_new_point(-0.5,1,0.5),1);
    middle->material->color = ft_new_color(0.1,1,0.5);
    middle->material->diffuse = 0.7;
    middle->material->specular = 0.3;

    t_sphere *right_s = default_sphere();
    right_s->transform = ft_multiply_matrix(ft_translate_matrix(ft_new_point(1.5,0.5,-0.5),1), ft_scaling_matrix(0.5,0.5,0.5,1),4,4);
    right_s->material->color = ft_new_color(0.5,1,0.1);
    right_s->material->diffuse = 0.7;
    right_s->material->specular = 0.3;

    t_sphere *l_sphere = default_sphere();
    l_sphere->transform = ft_multiply_matrix(ft_translate_matrix(ft_new_point(-1.5,0.33,-0.75), 1),ft_scaling_matrix(0.33,0.33,0.33,1),4,4);
    l_sphere->material->color = ft_new_color(1,0.8,0.1);
    l_sphere->material->diffuse = 0.7;
    l_sphere->material->specular = 0.3;

    world->light = ft_new_plight(ft_new_color(1,1,1), ft_new_point(-10,10,-10));


    world->shape;
    ft_add_shape(&world, floor, 0);
    ft_add_shape(&world, left_W, 0);
    ft_add_shape(&world, right_W, 0);
    ft_add_shape(&world, middle, 0);
    ft_add_shape(&world, right_s, 0);
    ft_add_shape(&world, l_sphere, 0);


    return world;
}

t_intersection *intersect_world(t_world *world, t_ray *ray)
{
    t_intersection *res;
    int i;
    int count;

    if (!world || !ray || !world->shape || world->n_objects <= 0)
        return NULL;
    t_shape *current = world->shape;
    t_intersection *inter = malloc(sizeof(t_intersection) * world->n_objects);
    if (!inter)
        return NULL;
    i = 0;
    count = 0;
    while (current && i < world->n_objects)
    {
        inter[i] = ft_intersect_sphere(ray, current->objects.sphere);
        count += inter[i].n_sol;
        current = current->next;
        i++;
    }
    res = ray_hit(inter, world->n_objects);
    if (res != inter)
        free(inter);
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

inline t_color *shading_hit(t_world *world, t_compose *comp)
{
    return (get_lighting_color(((t_sphere *) (comp->obj))->material, world->light, comp->point, comp->camv,comp->normv));
}

t_color *get_color_at(t_world *world, t_ray *ray) {
    t_intersection *inter;
    t_compose *comp;
    t_color *res;

    inter = intersect_world(world, ray);
    if (!inter) {
        printf("No intersection found\n");
        return ft_new_color(0, 0, 0);
    }
    
    // Debug intersection
    printf("Intersection at t = %f\n", inter->t1);
    
    comp = prepare_computations(inter, ray);
    
    // Debug normal and view vectors
    printf("Normal: (%f, %f, %f)\n", comp->normv->x, comp->normv->y, comp->normv->z);
    printf("View: (%f, %f, %f)\n", comp->camv->x, comp->camv->y, comp->camv->z);
    printf("Hit point: (%f, %f, %f)\n", comp->point->x, comp->point->y, comp->point->z);
    
    res = shading_hit(world, comp);
    
    // Debug final color
    printf("Color: (%f, %f, %f)\n", res->r, res->b, res->g);
    
    free(comp);
    free(inter);
    return res;
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

s_camera  *new_camera(float hsize, float vsize, float fov)
{
    s_camera *camera;
    float    aspect;
    float    half_view;

    camera = malloc(sizeof(s_camera));
    if (!camera)
        return NULL;
    camera->h_size = hsize;
    camera->v_size = vsize;
    camera->fov = fov;
    camera->transform = identity_matrix(4);
    aspect = camera->h_size / camera->v_size;
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


int render_image(t_scene *scene ,t_world *t_world, s_camera *cam)
{
    int x;
    int y;
    t_color *color;
    t_ray *ray;
    int pixel_color;
    for (y = 0; y < cam->h_size; y++)
    {
        for (int x = 0; x < cam->v_size; x++)
        {
            ray = get_ray_pixel(cam, x, y);
            color = get_color_at(t_world, ray);
            printf("%f %f %f\n", color->r, color->g, color->b);
            pixel_color = (255 << 24 | (int) (255 * color->r ) << 16 | (int) (255 * color->g) << 8| (int) color->b * 255);
            my_pixel_put(&scene->data->img, x, y, pixel_color);
        }
    }
    mlx_put_image_to_window(scene->data->mlx, scene->data->win, scene->data->img.img_ptr, 0, 0);
    mlx_hook(scene->data->win, 17, 0, &ft_close_window, scene->data);
    mlx_loop(scene->data->mlx);
}