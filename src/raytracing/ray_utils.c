#include "../../includes/minirt.h"

t_ray *create_ray(t_vector *origin, t_vector *direction)
{
    t_ray *new;
    
    new = malloc(sizeof(t_ray));
    if (!new)
        return (NULL);
    new->origin = origin;
    new->direction = direction;
    new->r_lab = vector_sub(direction, origin);
    new->color = NULL;
    return (new);
}

//to do

// a function that generate ray

/*
int gen_ray(t_scene *scene, float x, float y, t_ray **cam_ray) {
    // Calculate ray direction in viewport space
    t_vector *ray_direction = vector_add(
        scene->camera->projection_center,
        vector_add(
            vector_multiply_scalar(scene->camera->cam_u, x),
            vector_multiply_scalar(scene->camera->cam_v, y)
        )
    );
    
    // Normalize ray direction
    ray_direction = vector_normilze(ray_direction);
    
    // Create ray from camera position through calculated direction
    *cam_ray = create_ray(scene->camera->camera_position, ray_direction);
    
    return 1;
}

int close_enough(float p1, float p2)
{

    return (fabs(p1 - p2) < EPSILON);
}

int check_intersection(t_ray *cast_ray,t_vector *inter_point ,t_vector *local_normal,t_vector *locol_color)
{
    //calculating delata "delta = b^2 - 4 a c"

    //lign vector direction
    t_vector    *vhat ;

    vhat = vector_normilze(cast_ray->r_lab);

    // a will  be a unit vector that means it well allways equal to 1
    //calculate b
    float b = 2.0 * vector_dot(cast_ray->origin, vhat)  ;
    //calculate c
    float c = vector_dot(cast_ray->origin, cast_ray->origin) - 1.0 ; // raduis == 1 sphere->origin = cat_ray->origin
    // printf("==b || c == %f =||= %f ==\n",c , b);
    float inter_test = (b * b) - 4 * c;
    if (inter_test > 0.0)
    {
            double numSQRT = sqrtf(inter_test);
            double t1 = (-b + numSQRT) / 2.0;
            double t2 = (-b - numSQRT) / 2.0;
            
            /* If either t1 or t2 are negative, then at least part of the object is
                behind the camera and so we will ignore it.
            if ((t1 < 0.0) || (t2 < 0.0))
                return 0;
            else
            {
                // Determine which point of intersection was closest to the camera.
                if (t1 < t2)
                    inter_point = vector_add(cast_ray->origin, vector_multiply_scalar(vhat, (float)t1));
                else
                    inter_point = vector_add(cast_ray->origin , vector_multiply_scalar(vhat , (float)t2));
            }
            return 1;
    }
    return 0;
}
*/

int generate_ray(t_camera *camera, float pro_screen_x, float pro_screen_y, t_ray *camera_ray) {
    // Compute screen world coordinate
    t_vector *screen_world_part1 = vector_add(
        camera->projection_center,
        vector_multiply_scalar(camera->cam_u, pro_screen_x)
    );
    
    t_vector *screen_world_coordinate = vector_add(
        screen_world_part1,
        vector_multiply_scalar(camera->cam_v, pro_screen_y)
    );
    
    // Set ray properties
    camera_ray->origin = camera->camera_position;
    camera_ray->direction = screen_world_coordinate;
    camera_ray->r_lab = vector_sub(screen_world_coordinate, camera->camera_position);
    
    return 1;
}

// int check_intersection(t_ray *cast_ray, t_vector *inter_point, t_vector *local_normal, t_vector *local_color)
// {
//     t_vector *sphere_center = ft_new_vector(0, 0, 0);  // Origin sphere
//     float radius = 0.5;
    
//     t_vector *L = vector_sub(sphere_center, cast_ray->origin);
//     float tca = vector_dot(L, cast_ray->direction);
    
//     if (tca < 0) return 0;
    
//     float d2 = vector_dot(L, L) - (tca * tca);
//     if (d2 > (radius * radius)) return 0;
    
//     float thc = sqrtf(radius * radius - d2);
//     float t0 = tca - thc;
//     float t1 = tca + thc;
    
//     float t = (t0 < t1) ? t0 : t1;
    
//     // *inter_point = vector_add(cast_ray->origin, 
//     //                vector_multiply_scalar(cast_ray->direction, t));
    
//     // *local_normal = vector_normilze(vector_sub(*inter_point, sphere_center));
//     // *local_color = ft_new_vector(1.0, 0.0, 0.0);  // Red color
    
//     return 1;
// }