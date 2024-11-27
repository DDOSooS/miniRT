# include "../../includes/minirt.h"

/*
t_vector *ft_new_vector(float x, float y, float z)
{
    t_vector *new;

    new = malloc(sizeof(t_vector));
    if (!new)
        return (NULL);
    new->x = x;
    new->y = y;
    new->z = z;
    return (new);
}

*/
t_vector *vector_add(t_vector *v1, t_vector *v2)
{
    t_vector *vec_add;

    vec_add = ft_new_vector(0,0,0);
    if (!vec_add)
        return (NULL);
    vec_add->x = v1->x + v2->x;
    vec_add->y = v1->y + v2->y;
    vec_add->z = v1->z + v2->z;
    return (vec_add);
}

t_vector *vector_sub(t_vector *v1, t_vector *v2)
{
    t_vector *vec_sub;

    vec_sub = ft_new_vector(0,0,0);
    if (!vec_sub)
        return (NULL);
    vec_sub->x = v1->x - v2->x;
    vec_sub->y = v1->y - v2->y;
    vec_sub->z = v1->z - v2->z;
    return (vec_sub);
}

t_vector *vector_cross(t_vector *v1, t_vector *v2)
{
    t_vector *cross_v;

    cross_v = ft_new_vector(0,0,0);
    if (!cross_v)
        return (NULL);
    cross_v->x = v1->y * v2->z - v1->z * v2->y;
    cross_v->y = v1->z * v2->x - v1->x * v2->z;
    cross_v->z = v1->x *v2->y - v1->y * v2->x;
    return (cross_v);
}

float vector_dot(t_vector *v1, t_vector *v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

double vec_lenght(t_vector *v)
{
    return (sqrt(v->x * v->x + v->y * v->y + v->z * v->z));
}

t_vector *vector_multiply_scalar(t_vector *v, float scalar)
{
    t_vector *new;

    new = ft_new_vector(0,0,0);
    if (!new)
        return (NULL);
    new->x = v->x * scalar;
    new->y = v->y * scalar;
    new->z = v->z * scalar;
    return (new);
}

// t_vector *vector_normilze(t_vector *vec)
// {
//     t_vector *new;

//     float magnitude = vec_lenght(vec);
//     if (!magnitude || !vec)
//         return (NULL);
//     new = ft_new_vector(0,0,0);
//     new->x = vec->x / magnitude;
//     new->y = vec->y / magnitude;
//     new->z = vec->z / magnitude;
//     return (new);
// }

t_vector *vector_normilze(t_vector *vec) {
    if (!vec) // Check for NULL input vector.
        return NULL;

    float magnitude = vec_lenght(vec);
    if (magnitude == 0) // Check for zero magnitude to prevent division by zero.
        return ft_new_vector(0, 0, 0); // Return a zero vector.

    t_vector *new = ft_new_vector(0, 0, 0);
    if (!new)
        return NULL;

    new->x = vec->x / magnitude;
    new->y = vec->y / magnitude;
    new->z = vec->z / magnitude;

    return new;
}