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
    float a,b,c, discriminant;

    tmp = transform(ray, inverse_matrix(sphere->transform, 4));
    oc = vector_sub(tmp->origin, sphere->sphere_coordinates);
    a = vector_dot(tmp->direction, tmp->direction);
    b = 2 * vector_dot(tmp->direction, oc);
    c = vector_dot(oc, oc) - sphere->sphere_diameter * sphere->sphere_diameter;
    discriminant = b * b - 4 * a * c;
    intersection.object = (t_sphere *) sphere;
    intersection.type = 1;
    if (discriminant < 0)
    {
        intersection.n_sol = 0;
        intersection.t1 = 0;
        intersection.t2 = 0;
        return intersection;
    }
    if (discriminant == 0)
    {
        intersection.n_sol = 1;
        intersection.t1 = -b / (2 * a);
        intersection.t2 = 0;
    }
    if (discriminant > 0)
    {
        intersection.n_sol = 2;
        intersection.t1 = (-b - sqrt(discriminant)) / (2 * a);
        intersection.t2 = (-b + sqrt(discriminant)) / (2 * a);
    }
    if (intersection.t1 > intersection.t2)
    {
        float temp = intersection.t1;
        intersection.t1 = intersection.t2;
        intersection.t2 = temp;
    }
    return (intersection);
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

    dir = ft_multiply_matrix_vec(m, ray->direction);
    origin = ft_multiply_matrix_vec(m, ray->origin);
    return create_ray(origin, dir);
}