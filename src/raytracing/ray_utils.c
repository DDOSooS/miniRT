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
    obj_n = vector_sub(w_p, ft_new_point(0,0,0));
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
    sphere->material->color = ft_new_color(0.8, 1.0, 0.6);
    sphere->material->diffuse = 0.7;
    sphere->material->specular = 0.2;
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
t_shape *ft_new_shape(void *shape_obj, int shape_type) {
    t_shape *new_shape = malloc(sizeof(t_shape));
    if (!new_shape)
        return NULL;
    
    new_shape->type = shape_type;
    new_shape->next = NULL;

    if (shape_type == SHAPE_SPHERE) {
        new_shape->objects.sphere = (t_sphere *)shape_obj;
    } else if (shape_type == SHAPE_PLANE) {
        new_shape->objects.plane = (t_plane *)shape_obj;
    } else if (shape_type == SHAPE_CYLINDER) {
        new_shape->objects.cylinder = (t_cylinder *)shape_obj;
    } else {
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
    t_sphere *sphere1 = default_sphere();
    t_sphere *sphere2 = default_sphere();
    sphere2->transform = ft_scaling_matrix(0.5, 0.5, 0.5, 1);

    p_light *light1 = default_light();
    world->shape = NULL;
    world->light = light1;

    ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    ft_add_shape(&world, sphere2, SHAPE_SPHERE);
    printf("world shape n_object: %d\n", world->n_objects);
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
        printf("sol  -> %f---\n", inter[i].t1);
        printf("sol  -> %f---\n", inter[i].t2);
        current = current->next;
        i++;
    }
    printf("====== n COUNT SOLUTION %d ======= \n", count);
    res = ray_hit(inter, world->n_objects);
    if (res != inter)
        free(inter);
    printf("sol  -> %f---\n", res->t1);

    return res;
}

t_compose *prepare_computations(t_intersection inter, t_ray *ray)
{
    t_compose *comp;

    comp = malloc(sizeof(t_compose));

    comp->t = inter.t1;
    comp->obj = inter.object;
    comp->obj_type = inter.type;
    comp->camv = negate_vector(ray->direction);
    comp->point = position(ray, inter.t1);
    comp->normv = normilize_at_sphere_pos((t_sphere *)(inter.object),comp->point);
    if (vector_dot(comp->normv, comp->camv) < 0.0)
    {
        comp->inside = 1;
        comp->normv = negate_vector(comp->normv);
    }
    else
        comp->inside = 0;
    return comp;
}


// t_color *