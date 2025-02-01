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
    float min_t = INT_MAX;
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
    int t1, t2;
    sphere_to_ray = vector_sub(ray->origin, sphere->sphere_coordinates);
    a = vector_dot(ray->direction, ray->direction);
    b = 2 * vector_dot(ray->direction, sphere_to_ray);
    c = vector_dot(sphere_to_ray, sphere_to_ray) - (sphere->sphere_diameter/2.0 * sphere->sphere_diameter/2.0);
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        result.t1 = -1;
        // result.t2 = -1;
        result.n_sol = 0;
    }
    else
    {
        t1 = (-b - sqrt(discriminant)) / (2 * a);
        t2 = (-b + sqrt(discriminant)) / (2 * a);
        if (t1 > 0 && t2 > 0 && t1 > t2)
            t1 = t2;
        result.t1 = t1;
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
    // intersection.t2 = t;
    intersection.object = object;
    intersection.type = type;
    return (intersection);
}

t_ray *transform(t_ray *ray, float **m)
{
    t_vector *dir;
    t_point *origin;

    dir = ft_multiply_matrix_vec(m, ray->direction);
    origin = ft_multiply_matrix_vec(m, ray->origin);
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

t_intersection ft_intersect_plane(t_ray *ray, t_plane *plane)
{
    t_intersection result = {0,  0, NULL, 0};
    
    // 1. Calculate denominator (dot product of ray direction and plane normal)
    float denom = vector_dot(ray->direction, plane->plane_normal);
    // 2. Check for parallel ray (denominator near zero)
    if (fabs(denom) < EPSILON)  
        return result;
    // 3. Calculate vector from ray origin to a point on the plane
    t_vector *origin_to_plane = vector_sub(ray->origin, plane->plane_cordinates);  // Fixed: use plane->point instead of plane->plane_normal
    // 4. Calculate intersection distance
    float t = -(vector_dot(origin_to_plane, plane->plane_normal)) / denom;
    free(origin_to_plane);
    if (t < EPSILON)
        return result;  // Intersection is behind ray origin
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
    
    x = ray->origin->x  + t * ray->direction->x;
    z = ray->origin->z  + t * ray->direction->z;
    return (x*x + z*z <= radius);
}

t_intersection check_intersection_caps(t_ray *ray, t_cylinder *cylinder, float cap)
{
    t_intersection result = {-1, -1, NULL, SHAPE_CYLINDER};
    
    if (fabs(ray->direction->y) < EPSILON)
        return result;
        
    float xs = (cap - ray->origin->y) / ray->direction->y;
    
    if (xs < EPSILON)
        return result;
        
    float x = ray->origin->x + xs * ray->direction->x;
    float z = ray->origin->z + xs * ray->direction->z;
    
    float radius_squared = (cylinder->raduis + EPSILON) * (cylinder->raduis + EPSILON);
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









/*
double          n_ray_cylinder(t_ray *ray, t_cylinder *cylinder, t_data *d)
{
    double a;
    double b;
    double c;
    double delta;
    double root;
    double deg;
    deg = M_PI * 45 / 180;

    a = ray->dir->x * ray->dir->x + cos(deg) * cos(deg) * ray->dir->z * ray->dir->z -
            2 * ray->dir->z * cos(deg) * ray->dir->y * sin(deg) + ray->dir->y * ray->dir->y *
            sin(deg) * sin(deg);
    b =  2 * (ray->ori->x - cylinder->base->x) * ray->dir->x + 2 * cos(deg) * cos(deg) * (ray->ori->z - cylinder->base->z) * ray->dir->z -
            2 * (ray->ori->z - cylinder->base->z) * cos(deg) * ray->dir->y * sin(deg) - 2 * ray->dir->z * cos(deg) *
            (ray->ori->y - cylinder->base->y) * sin(deg) + 2 * (ray->ori->y - cylinder->base->y) * ray->dir->y * sin(deg) * sin(deg);
    c = (ray->ori->x - cylinder->base->x) * (ray->ori->x - cylinder->base->x) + (ray->ori->z - cylinder->base->z) * (ray->ori->z - cylinder->base->z)* cos(deg) * cos(deg) -
            2 * (ray->ori->z - cylinder->base->z) * cos(deg) * (ray->ori->y - cylinder->base->y) * sin(deg) + (ray->ori->y - cylinder->base->y) * (ray->ori->y - cylinder->base->y) *
            sin(deg) * sin(deg) - cylinder->radius * cylinder->radius;
    delta = b * b -  (4 * a * c);
    if (delta > ACC)
    {
            root = (-1 * b - sqrt(delta)) / (2 * a) - ACC;
            if (root <= ACC)
                    root = (-1 * b + sqrt(delta)) / (2 * a) - ACC;
            return (root);
    }
    return (-1);
}



*/


t_intersection ft_intersect_cylinder(t_ray *ray, t_cylinder *cylinder)
{
    t_intersection result = {-1, -1, NULL, SHAPE_CYLINDER};
    t_intersection captop, capbottom;
    float half_height = cylinder->height / 2.0f;
    float sol[4];
    int counter = 0;

    // Get inverse transformation matrix
    float **inverse = inverse_matrix(cylinder->transform, 4);
    if (!inverse)
        inverse = identity_matrix(4);

    // Transform ray to object space
    t_ray *transformed_ray = malloc(sizeof(t_ray)); // Allocate memory for transformed ray
    if (!transformed_ray)
    {
        // ft_free_matrix(inverse);
        return result;
    }

    if (!ray || !ray->origin || !ray->direction)
    {
        // ft_free_matrix(inverse);
        free(transformed_ray);
        return result;
    }

    transformed_ray->origin = ft_multiply_matrix_vec(inverse, ray->origin);
    transformed_ray->direction = ft_multiply_matrix_vec(inverse, ray->direction);
    
    if (!transformed_ray->origin || !transformed_ray->direction)
    {
        // ft_free_matrix(inverse);
        free(transformed_ray);
        return result;
    }

    // Now use the transformed ray for calculations
    float ox = transformed_ray->origin->x;
    float oy = transformed_ray->origin->y;
    float oz = transformed_ray->origin->z;
    float dx = transformed_ray->direction->x;
    float dy = transformed_ray->direction->y;
    float dz = transformed_ray->direction->z;

    float a = dx * dx + dz * dz;
    // Avoid division by zero
    if (fabs(a) < EPSILON)
    {
        // ft_free_matrix(inverse);
        free(transformed_ray->origin);
        free(transformed_ray->direction);
        free(transformed_ray);
        return result;
    }

    float b = 2.0f * (ox * dx + oz * dz);
    float c = ox * ox + oz * oz - cylinder->raduis * cylinder->raduis;
    float discriminant = b * b - 4.0f * a * c;
    
    if (discriminant >= 0)
    {
        float sqrt_disc = sqrtf(discriminant);
        float t1 = (-b - sqrt_disc) / (2.0f * a);
        float t2 = (-b + sqrt_disc) / (2.0f * a);

        float y1 = transformed_ray->origin->y + t1 * dy;
        float y2 = transformed_ray->origin->y + t2 * dy;

        float maximum = half_height;
        float minimum = -half_height;

        if (y1 >= minimum && y1 <= maximum)
            sol[counter++] = t1;
        if (y2 >= minimum && y2 <= maximum) 
            sol[counter++] = t2;
    }

    captop = check_intersection_caps(transformed_ray, cylinder, half_height);
    if (captop.n_sol > 0)
        sol[counter++] = captop.t1;
    capbottom = check_intersection_caps(transformed_ray, cylinder, -half_height);
    if (capbottom.n_sol > 0)
        sol[counter++] = capbottom.t1;

    // Clean up
    // ft_free_matrix(inverse);
    free(transformed_ray->origin);
    free(transformed_ray->direction);
    free(transformed_ray);

    if (counter > 0)
    {
        float min_t = get_min_sol(sol, counter);
        if (min_t > EPSILON)
        {
            result.n_sol = 1;
            result.object = cylinder;
            result.t1 = min_t;
        }
    }
    return result;
}

t_intersection *intersect_world(t_world *world, t_ray *ray)
{
    t_intersection *res;
    int i;

    if (!world->shape)
        return NULL;
    t_shape *current = world->shape;
    t_intersection *inter = malloc(sizeof(t_intersection) * world->n_objects);
    if (!inter)
        return NULL;
    i = 0;
    while (current)
    {
        if (current->type == SHAPE_SPHERE)
            inter[i] = ft_intersect_sphere(ray, current->objects.sphere);
        else if (current->type == SHAPE_PLANE)
            inter[i] = ft_intersect_plane(ray, current->objects.plane);
        else if (current->type == SHAPE_CYLINDER)
            inter[i] = ft_intersect_cylinder(ray, current->objects.cylinder);  
        current = current->next;
        i++;
    }
    res = ray_hit(inter, world->n_objects);
    return res;
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

t_vector *normalize_at_plane_pos(t_plane *plane, t_point *w_p)
{
    t_vector *vec, *tmp;
    t_vector *obj_n;
    float **inv_m;
    
    obj_n = vector_normilze(plane->plane_normal); 
    inv_m = inverse_matrix(plane->transform, 4);
    if (!inv_m) 
        inv_m = plane->transform; 
    ft_transpose_matrix(&inv_m, 4, 4);
    tmp = ft_multiply_matrix_vec(inv_m, obj_n);
    tmp->w = 0; 
    vec = vector_normilze(tmp);
    free(tmp);
    free(obj_n);
    if (inv_m != plane->transform)
        ft_free_matrix(inv_m, 4);
    return vec;
}


t_vector *normalize_at_cylinder_pos(t_cylinder *cylinder, t_point *world_p)
{
    float **inv = inverse_matrix(cylinder->transform,4);
    if (!inv)
        inv = cylinder->transform;
    t_point *local_p = ft_multiply_matrix_point(inv, world_p);

    // Calculate normal in object space 
    t_vector local_normal;
    if (fabs(local_p->y - cylinder->height/2) < EPSILON)
        local_normal = (t_vector){0, 1, 0};
    else if (fabs(local_p->y + cylinder->height/2) < EPSILON)
        local_normal = (t_vector){0, -1, 0};
    else
        local_normal = (t_vector){local_p->x, 0, local_p->z};

    // Transform normal back to world space
    ft_transpose_matrix(&inv,4,4);
    t_vector *world_normal = ft_multiply_matrix_vec(inv, &local_normal);
    vector_normilze(world_normal);
    return world_normal;
}

t_compose *prepare_computations(t_intersection *inter, t_ray *ray)
{
    t_compose *comp = malloc(sizeof(t_compose));

    comp->t = inter->t1;
    comp->obj = inter->object;
    comp->obj_type = inter->type;
    comp->camv = negate_vector(ray->direction);
    comp->point = position(ray, comp->t);
    if (comp->obj_type == SHAPE_SPHERE)
        comp->normv = normilize_at_sphere_pos((t_sphere *)(inter->object), comp->point);
    else if (comp->obj_type == SHAPE_PLANE)
        comp->normv = normalize_at_plane_pos((t_plane *)(inter->object), comp->point);
    else if (comp->obj_type == SHAPE_CYLINDER)
        comp->normv = normalize_at_cylinder_pos((t_cylinder *)(inter->object), comp->point);
    comp->normv = vector_normilze(comp->normv);
    if (vector_dot(comp->normv, comp->camv) < 0.0)
    {
        comp->inside = 1;
        comp->normv = negate_vector(comp->normv);
    }
    else
        comp->inside = 0;
    return comp;
}


float **get_view_transform(t_point *from_v, t_vector *to_v, t_vector *up_v)
{
    float **view_transform;
    t_vector *forward_v;
    t_vector *left_v;
    t_vector *up_n;
    float **res;

    // forward_v = vector_sub(to_v, from_v);
    forward_v = vector_normilze(to_v);
    up_n = vector_normilze(up_v);
    left_v = vector_cross(forward_v, up_n);
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
    res = ft_multiply_matrix(view_transform, ft_translate_matrix(ft_new_point(-from_v->x, -from_v->y, -from_v->z), 1), 4, 4);
    free(forward_v);
    free(left_v);
    free(up_n);
    free(up_v);
    free(view_transform);
    return res;
}

float **create_rotation_matrix_from_vector(t_vector *orientation)
{
    t_vector *normalized = vector_normilze(orientation);
    double theta_y = atan2(normalized->y, normalized->z);
    double theta_x = atan2(-normalized->x, sqrt(normalized->y * normalized->y + normalized->z * normalized->z));
    
    float **rot_x = rotate_x(theta_x);
    float **rot_y = rotate_y(theta_y);
    
    // Combine rotations (multiply matrices)
    return ft_multiply_matrix(rot_y, rot_x,4,4);
}


t_world *default_world()
{
    t_world *world = malloc(sizeof(t_world));
    if (!world)
        return NULL;
    world->n_objects = 0;
    world->shape = NULL;

    t_sphere *sphere1 = default_sphere();
    sphere1->sphere_diameter = 30; 
    sphere1->sphere_coordinates = ft_new_point(0, 0, 0);
    sphere1->material->color = ft_new_color(0.9, 0.5, 0.2);


    t_sphere *sphere2 = default_sphere();
    sphere2->sphere_diameter = 40; 
    sphere2->sphere_coordinates = ft_new_point(50, 0, 0);
    sphere2->material->color = ft_new_color(1, 0, 0.2);
    sphere2->transform = identity_matrix(4);

    // t_sphere *sphere3 = default_sphere();
    // sphere3->sphere_diameter = 60; 
    // sphere3->sphere_coordinates = ft_new_point(0, , -20);
    // sphere3->material->color = ft_new_color(0.9, 0.5, 0.2);
    
    t_plane *plane2 = malloc(sizeof(t_plane));
    plane2->plane_normal = ft_new_vector(0, 0, -1);
    plane2->plane_cordinates = ft_new_point(0,1, 100);
    plane2->plane_color = ft_new_color(0.8, 0.8, 0.8);
    plane2->transform = identity_matrix(4);
    plane2->material = default_material();
    plane2->material->color = ft_new_color(1, 1, 1);
    plane2->material->diffuse = 0.7;
    plane2->material->specular = 0.3;

    t_plane *plane = malloc(sizeof(t_plane));
    plane->plane_normal = ft_new_vector(0, -1, 0);
    plane->plane_cordinates = ft_new_point(0,-30, -10);
    plane->plane_color = ft_new_color(0.8, 0.8, 0.8);
    plane->transform = identity_matrix(4);
    plane->material = default_material();
    plane->material->color = ft_new_color(1, 0, -10);
    plane->material->diffuse = 0.7;
    plane->material->specular = 0.3;

    t_cylinder *cylinder1 = malloc(sizeof(t_cylinder));
    cylinder1->raduis = 10;
    cylinder1->height = 80;
    cylinder1->material = default_material();
    cylinder1->material->color = ft_new_color(1, 0, 0);
    cylinder1->orientation = ft_new_vector(0,0,0);
    cylinder1->transform = create_rotation_matrix_from_vector(cylinder1->orientation);
    cylinder1->coordinates = ft_new_point(-0,0,0);
    cylinder1->next = NULL;

    // Light 
    world->light = ft_new_plight
    (
        ft_new_color(1, 1, 1),
        ft_new_point(0, 70, -100)
    );
    // ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    ft_add_shape(&world, plane, SHAPE_PLANE);
    ft_add_shape(&world, plane2, SHAPE_PLANE);
    ft_add_shape(&world, cylinder1, SHAPE_CYLINDER);
    // if (world->shape->objects.cylinder)
    // printf("cylinder is  being created. height:%d==\n", world->shape->objects.cylinder->height);
    // ft_add_shape(&world, sphere1, SHAPE_SPHERE);
    // ft_add_shape(&world, sphere2, SHAPE_SPHERE);
    return world;
}

s_camera *new_camera(float h_size, float w_size, float fov, t_point *p, t_vector *dir)
{
    s_camera *camera;
    float aspect;
    float half_view;
   
    camera = malloc(sizeof(s_camera));
    if (!camera)
        return NULL;
    camera->origin = p;
    camera->direction = dir;
    camera->h_size = h_size;
    camera->w_size = w_size;
    aspect = camera->w_size / camera->h_size;
    camera->fov = fov; 
    half_view = tan(fov / 2.0f);
    if (aspect >= 1.0f)
    {
        camera->half_w_size = half_view;
        camera->half_h_size = half_view / aspect;
    }
    else
    {
        camera->half_w_size = half_view * aspect;
        camera->half_h_size = half_view;
    }
    camera->pixel_size = camera->half_w_size * 2.0f / camera->w_size;
    camera->transform = get_view_transform(p,  vector_normilze(dir), ft_new_vector(0, 1, 0));
    // Calculate the size of a single pixel in world units
    // printf("pixel_size = %f \n", camera->pixel_size);
    return camera;
}

t_ray *get_ray_pixel(s_camera *cam, float x, float y, float edge)
{
    t_ray *ray;
    t_point *pixel_world;
    t_point *pixel;
    float world_x, world_y;
    float **inv;

    ray = malloc(sizeof(t_ray));
    if (!ray)
        return NULL;

    world_x = cam->half_w_size - (x + edge) * cam->pixel_size;
    world_y = cam->half_h_size - (y + 0.5) * cam->pixel_size;

    inv = inverse_matrix(cam->transform, 4);
    if (!inv)
        inv = cam->transform;

    pixel = ft_new_point(-world_x, -world_y, -1);
	pixel_world = ft_multiply_matrix_vec(inv, pixel);
    free(pixel);
    ray->origin = ft_multiply_matrix_vec(inv, ft_new_point(0, 0, 0));
	ray->direction = vector_normilze(vector_sub(pixel_world, ray->origin));
    free(pixel_world);
    // if(inv != cam->transform)
    // free_matrix(inv);
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
        // free_intersection(intersections);
        return 1;
    }
    free(v);
    free(direction);
    free(r);
    // free_intersection(intersections);
    return 0;
}


t_color *shading_hit(t_world *world, t_compose *comp)
{
    t_color *color;
    t_material *material;
    int shadowed ;

    if (comp->obj_type == SHAPE_SPHERE)
    {
        material = ((t_sphere *)comp->obj)->material;
    }
    else if (comp->obj_type == SHAPE_PLANE)
        material = ((t_plane *)comp->obj)->material;
    else if (comp->obj_type == SHAPE_CYLINDER)
        material = ((t_cylinder *)comp->obj)->material;
    shadowed = is_shadowed(world, comp->point);
    color = get_lighting_color(material, world->light, comp->point,
                             comp->camv, comp->normv, shadowed);
    return color;
}

t_color *get_color_at(t_world *world, t_ray *ray)
{
    t_intersection *inter;
    t_compose *comp;
    t_color *res;

    inter = intersect_world(world, ray);
    if (!inter || inter->n_sol <= 0)
    {
        return ft_new_color(0, 0, 0);
    }

    comp = prepare_computations(inter, ray);
    res = shading_hit(world, comp);
    // free_compose(comp);
    // free_intersection(inter);
    return res;
}


int render_image(t_scene *scene, t_world *world, s_camera *cam)
{
    float x, y;
    t_color *color;
    t_ray *ray;
    int pixel_color;
    float edge;

    for (y = 0; y < cam->h_size; y++)
    {
        for (x = 0; x < cam->w_size; x++)
        { 
            edge = 0.5;
                ray = get_ray_pixel(cam, x, y, edge);
                color = get_color_at(world, ray);
                pixel_color = (255 << 24) |
                            ((int)(255 * color->r) << 16) |
                            ((int)(255 * color->g) << 8) |
                            ((int)(255 * color->b) << 0);
                my_pixel_put(&scene->data->img, x, y, pixel_color);
            // for (int j = 0; j < 2; j++)
            // {
            //     edge += 0.5;
            // }
            // free(ray);
            // free(color);
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