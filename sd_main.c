#include  "./includes/minirt.h"
#define XK_escape 65307
#define L_key 65361
#define U_key 65362
#define R_key 65363
#define D_key 65364

#define BACKGROUND_COLOR 50

void var_dump_all(t_map *map, t_scene *scene)
{
    // ---  t_scene ---
    if (scene)
    {
        //  camera
        printf("\n--- Camera  %p---\n" , scene->camera);
        if (scene->camera)
        {
            printf("Camera Position: [%f, %f, %f]\n", scene->camera->camera_position->x, scene->camera->camera_position->y, scene->camera->camera_position->z);
            printf("Camera Coordinates: [%f, %f, %f]\n", scene->camera->camera_dir->x, scene->camera->camera_dir->y, scene->camera->camera_dir->z);
            printf("Camera FOV: %d\n", scene->camera->camera_fov);
        } else {
            printf("Camera is NULL\n");
        }

        //  ambient
        printf("\n--- Ambient ---\n");
        if (scene->ambient)
        {
            printf("Ambient Ratio: %f\n", scene->ambient->ambient_ration);
            printf("Ambient Color: [%d, %d, %d]\n", scene->ambient->ambient_color->r, scene->ambient->ambient_color->g, scene->ambient->ambient_color->b);
        } else {
            printf("Ambient is NULL\n");
        }

        //  light
        printf("\n--- Light ---\n");
        if (scene->light) {
            printf("Light Coordinates: [%f, %f, %f]\n", scene->light->light_coordinate->x, scene->light->light_coordinate->y, scene->light->light_coordinate->z);
            printf("Light Ratio: %f\n", scene->light->light_ration);
            printf("Light Color: [%d, %d, %d]\n", scene->light->light_color->r, scene->light->light_color->g, scene->light->light_color->b);
        } else {
            printf("Light is NULL\n");
        }

        //  spheres
        printf("\n--- Spheres ---\n");
        t_sphere *sphere = scene->sphere;
        if (sphere) {
            while (sphere) {
                printf("Sphere Coordinates: [%f, %f, %f]\n", sphere->sphere_coordinates->x, sphere->sphere_coordinates->y, sphere->sphere_coordinates->z);
                printf("Sphere Diameter: %f\n", sphere->sphere_diameter);
                printf("Sphere Color: [%d, %d, %d]\n", sphere->sphere_color->r, sphere->sphere_color->g, sphere->sphere_color->b);
                sphere = sphere->next;
            }
        } else {
            printf("No Spheres in Scene\n");
        }

        //  planes
        printf("\n--- Planes ---\n");
        t_plane *plane = scene->plane;
        if (plane) {
            while (plane) {
                printf("Plane Coordinates: [%f, %f, %f]\n", plane->plane_cordinates->x, plane->plane_cordinates->y, plane->plane_cordinates->z);
                printf("Plane Normal: [%f, %f, %f]\n", plane->plane_normal->x, plane->plane_normal->y, plane->plane_normal->z);
                printf("Plane Color: [%d, %d, %d]\n", plane->plane_color->r, plane->plane_color->g, plane->plane_color->b);
                plane = plane->next;
            }
        } else {
            printf("No Planes in Scene\n");
        }

        //  cylinders
        printf("\n--- Cylinders ---\n");
        t_cylinder *cylinder = scene->cylinder;
        if (cylinder) {
            while (cylinder) {
                printf("Cylinder Coordinates: [%f, %f, %f]\n", cylinder->coordinates->x, cylinder->coordinates->y, cylinder->coordinates->z);
                printf("Cylinder Orientation: [%f, %f, %f]\n", cylinder->orientation->x, cylinder->orientation->y, cylinder->orientation->z);
                printf("Cylinder raduis: %f, Height: %f\n", cylinder->raduis, cylinder->height);
                printf("Cylinder Color: [%d, %d, %d]\n", cylinder->cylinder_color->r, cylinder->cylinder_color->g, cylinder->cylinder_color->b);
                cylinder = cylinder->next;
            }
        } else {
            printf("No Cylinders in Scene\n");
        }
    } else {
        printf("Scene is NULL\n");
    }
}

t_map *ft_init_map()
{
    t_map *new;
    int i;

    i = -1;
    new = (t_map *)malloc(sizeof(t_map));
    if (!new)
        return NULL;
    new->lines = NULL;
    while (++i < 3)
        new->scen_elements[i] = 0;    
    return new;
}

int	create_trgb(int t, int r, int g, int b)
{
	return (t << 24 | r << 16 | g << 8 | b);
}

void my_pixel_put(t_img *img, int x, int y, int color)
{
    int offset;

    offset = (img->line_length * y) + (x * (img->bits_per_pixel / 8));
    *((unsigned int *)(img->addr + offset)) = color;
}

void var_dump_vector(t_vector *vec)
{
    printf("x-> (%f) y-> (%f) z-> (%f) ===\n",vec->x, vec->y, vec->z);
}

/*
int check_sphere_intersection(t_ray *ray, t_sphere *sphere)
{
    t_vector    *oc;
    double      a, b, c, delta;

    oc = vector_sub(sphere->sphere_coordinates, ray->origin);
    // Debug print
    printf("Ray direction: x=%f, y=%f, z=%f\n", ray->direction->x, ray->direction->y, ray->direction->z);
    printf("Sphere pos: x=%f, y=%f, z=%f\n", sphere->sphere_coordinates->x, 
           sphere->sphere_coordinates->y, sphere->sphere_coordinates->z);
    
    a = 1;
    b = -2.0 * vector_dot(ray->direction, oc);
    c = vector_dot(oc, oc) - sphere->sphere_diameter * sphere->sphere_diameter;
    delta = b * b - 4 * a * c;
    printf("=== delta>> a=%f b=%f c=%f  delta::%f<<====\n", a,b,c,delta);
    return (delta >= 0);  
}
Add this helper function
double get_sphere_intersection_t(t_ray *ray, t_sphere *sphere)
{
    t_vector *oc = vector_sub(ray->origin, sphere->sphere_coordinates);
    double a = vector_dot(ray->direction, ray->direction);
    double b = 2.0 * vector_dot(ray->direction, oc);
    double c = vector_dot(oc, oc) - (sphere->sphere_diameter * sphere->sphere_diameter / 4.0);
    double delta = b * b - 4 * a * c;
    
    if (delta < 0) return -1;
    
    double t1 = (-b - sqrt(delta)) / (2.0 * a);
    double t2 = (-b + sqrt(delta)) / (2.0 * a);
    
    return (t1 < t2 && t1 > 0) ? t1 : t2;
}

int check_intersection(t_ray *ray, t_scene *scene)
{
    t_vector *unit_dir = vector_normilze(ray->direction);
    double a = 0.5 * (unit_dir->y + 1.0);
    
    // Get the actual intersection distance
    double t = get_sphere_intersection_t(ray, scene->sphere);
    printf("intersection === %f====\n", t);
    if (t >= 0)  // If we hit the sphere
    {
        // Calculate intersection point and normal for proper shading
        t_vector *hit_point = vector_add(ray->origin, vector_multiply_scalar(ray->direction, t));
        t_vector *normal = vector_normilze(vector_sub(hit_point, scene->sphere->sphere_coordinates));
        
        // Simple diffuse shading
        // double light_intensity = fmax(0.2, vector_dot(normal, scene->light->light_coordinate));
        
        return (255 << 24 | 
                (int)(scene->sphere->sphere_color->r) << 16 | 
                (int)(scene->sphere->sphere_color->b) << 8 | 
                (int)(scene->sphere->sphere_color->b));
    }

    // Background color if no hit
    int red   = (int)(255.0 * ((1.0 - a) * 1.0 + a * 0.5));
    int green = (int)(255.0 * ((1.0 - a) * 1.0 + a * 0.7));
    int blue  = (int)(255.0 * ((1.0 - a) * 1.0 + a * 1.0));
    
    return (255 << 24 | red << 16 | green << 8 | blue);
}

void update_camera_settings(t_scene *scene)
{
    scene->camera->focal_lenght = 1.0;
    t_vector *vec_up = ft_new_vector(0,1,0);
    // Calculate viewport vectors
    double viewport_height = 2.0 * tan(scene->camera->camera_fov * PI / 360.0);
    double viewport_width = viewport_height * scene->aspect_ratio;
    
    // Get camera orientation vectors
    t_vector *w = vector_normilze(vector_multiply_scalar(scene->camera->camera_dir, -1));
    t_vector *u = vector_normilze(vector_cross(vec_up, w));
    t_vector *v = vector_cross(w, u);
    
    // Calculate viewport vectors
    scene->vp_u = vector_multiply_scalar(u, viewport_width);
    scene->vp_v = vector_multiply_scalar(v, -viewport_height);
    
    // Calculate pixel delta vectors
    scene->camera->cam_u = vector_multiply_scalar(scene->vp_u, 1.0 / scene->image_width);
    scene->camera->cam_v = vector_multiply_scalar(scene->vp_v, 1.0 / scene->image_height);
    
    // Calculate upper left corner
    t_vector *viewport_center = vector_sub(scene->camera->camera_position, w);
    t_vector *viewport_upper_left = vector_sub(viewport_center, 
                                             vector_add(vector_multiply_scalar(scene->vp_u, 0.5),
                                                      vector_multiply_scalar(scene->vp_v, 0.5)));
    
    scene->l_corner = vector_add(viewport_upper_left,
                                vector_multiply_scalar(vector_add(scene->camera->cam_u,
                                                               scene->camera->cam_v),
                                                    0.5));
}
*/


t_vector *pixel_dir(int x, int y, t_scene *scene)
{
    
    double pixel_x = x - scene->image_width / 2.0;
    double pixel_y = - (y - scene->image_height / 2.0);
    double pixel_z = scene->camera->focal_lenght;  
    t_vector *dir = ft_new_vector(pixel_x, pixel_y, pixel_z);
    return vector_normilze(dir);  
}

double intersection(t_vector *u, t_vector *sphere_center, t_scene *scene)
{
    double  t1;
    double  t2;
    double  delta, a, b, c;

    t_vector *camera_to_center = vector_sub(sphere_center, scene->camera->camera_position);
    a = 1.0;  
    b = -2.0 * vector_dot(u, camera_to_center);
    c = vector_dot(camera_to_center, camera_to_center) - 
               (scene->sphere->sphere_diameter * scene->sphere->sphere_diameter);
    free(camera_to_center);  
    delta = b * b - 4.0 * a * c;
    if (delta < 0)
        return 0;     
    t1 = (-b - sqrt(delta)) / (2.0 * a);
    t2 = (-b + sqrt(delta)) / (2.0 * a);
    if (t1 > 0 && t2 > 0)
        return fmin(t1, t2);
    if (t1 > 0)
        return t1;
    if (t2 > 0)
        return t2;
    return 0;
}

t_vector *calculate_surface_normal(t_vector *intersection_point, t_vector *sphere_center)
{
    t_vector *normal = vector_sub(intersection_point, sphere_center);
    return vector_normilze(normal);
}

int get_pixel_color(t_vector *dir, t_scene *scene)
{
    double t = intersection(dir, scene->sphere->sphere_coordinates, scene);
    
    if (t > 0)
    {
        t_vector *intersection_point = vector_multiply_scalar(dir, t);
        
        t_vector *normal = calculate_surface_normal(intersection_point, 
                                                    scene->sphere->sphere_coordinates);
        
        //  light direction vector
        t_vector *light_dir = vector_sub(scene->light->light_coordinate, intersection_point);
        t_vector *light_dir_normalized = vector_normilze(light_dir);
        
        //  light intensity (
        double light_intensity = fmax(vector_dot(normal, light_dir_normalized), 0.0);
        
        //  sphere color => light intensity and light ratio
        int r = (int)fmin(scene->sphere->sphere_color->r * light_intensity * scene->light->light_ration, 255);
        int g = (int)fmin(scene->sphere->sphere_color->g * light_intensity * scene->light->light_ration, 255);
        int b = (int)fmin(scene->sphere->sphere_color->b * light_intensity * scene->light->light_ration, 255);
        
        // Blend with light color
        r = (r * scene->light->light_color->r) / 255;
        g = (g * scene->light->light_color->g) / 255;
        b = (b * scene->light->light_color->b) / 255;
        free(intersection_point);
        free(normal);
        free(light_dir);
        free(light_dir_normalized);
        return (255 << 24) | (r << 16) | (g << 8) | b;
    }
    
    return (255 << 24) | (50 << 16) | (50 << 8) | 50;
}


void init_scene(t_scene *scene)
{
    scene->image_width = 700;
    scene->aspect_ratio = 16.0f / 9.0f;
    scene->image_height = scene->image_width / scene->aspect_ratio;
    scene->vp_hight = 2.0;
    scene->vp_width = scene->vp_hight * scene->aspect_ratio;
    scene->camera->focal_lenght = 500; 
    
    scene->data = malloc(sizeof(t_var));
    scene->data->mlx = mlx_init();
    scene->data->win = mlx_new_window(scene->data->mlx, 
                                     scene->image_width, 
                                     scene->image_height, 
                                     "MiniRT");
    scene->data->img.img_ptr = mlx_new_image(scene->data->mlx, 
                                            scene->image_width, 
                                            scene->image_height);
    scene->data->img.addr = mlx_get_data_addr(scene->data->img.img_ptr,
                                             &scene->data->img.bits_per_pixel,
                                             &scene->data->img.line_length,
                                             &scene->data->img.endian);
}

int	ft_close(t_var *vars)
{
    printf("============> (hehooooooooooo) <============\n");
	mlx_destroy_window(vars->mlx, vars->win);
	exit(0); // Exit the program
	return (0);
}

int	ft_close_window(t_var *data)
{
    mlx_destroy_window(data->mlx, data->win);

	exit(EXIT_SUCCESS);
	return (1);
}

int render_sphere(t_scene *scene)
{
    t_vector *dir;
    int color;

    for (int y = 0; y < scene->image_height; y++) 
    {
        for (int x = 0; x < scene->image_width; x++)
        {
            dir = pixel_dir(x, y, scene);
            color = get_pixel_color(dir, scene);
            my_pixel_put(&scene->data->img, x, y, color);
            free(dir);
        }
    }
    
    mlx_put_image_to_window(scene->data->mlx, 
                           scene->data->win, 
                           scene->data->img.img_ptr, 
                           0, 0);
    mlx_hook(scene->data->win, 17, 32, &ft_close_window, scene->data);
    mlx_loop(scene->data->mlx);
    return 1;
}

void print_matrix(float **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%8.6f ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv)
{
    t_map *map;
    t_scene *scene;

    if (argc != 2)
        return (printf("error: try with : ./minirt scene.rt\n"), 1);
    map = ft_init_map();
    if (!map)
        return (printf("error : memory allocation failed\n"), 1);
    map->lines = ft_gen_scen_map(argv[1]);
    if (!map->lines || !ft_check_map_components(&map))
        return (free(map), ft_putstr_fd("map is empty\n", 2), 1);
/*
    scene = ft_generate_scene(map->lines);
    
    var_dump_all(map, scene);
    init_scene(scene);
    render_sphere(scene);
    float **matrix = ft_create_matrix(4,4);
    matrix[0][0]=3.0f;
    matrix[0][1]=-9.0f;
    matrix[0][2]=7.0f;
    matrix[0][3]=3.0f;
    matrix[1][0]=3.0f;
    matrix[1][1]=-8.0f;
    matrix[1][2]=2.0f;
    matrix[1][3]=-9.0f;
    matrix[2][0]=-4.0f;
    matrix[2][1]=4.0f;
    matrix[2][2]=4.0f;
    matrix[2][3]=1.0f;
    matrix[3][0]=-6.0f;
    matrix[3][1]=5.0f;
    matrix[3][2]=-1.0f;
    matrix[3][3]=1.0f;
    for(int i= 0; i <2; i++)
        for (int j= 0; j< 2; j++)
            printf("matrix[%d][%d]==[%f]\n", i, j, matrix[i][j]);
    float **matrix1 = ft_create_matrix(4,4);
    matrix1[0][0]=8.0f;
    matrix1[0][1]=2.0f;
    matrix1[0][2]=2.0f;
    matrix1[0][3]=2.0f;
    matrix1[1][0]=3.0f;
    matrix1[1][1]=-1.0f;
    matrix1[1][2]=7.0f;
    matrix1[1][3]=-0.0f;
    matrix1[2][0]=7.0f;
    matrix1[2][1]=0.0f;
    matrix1[2][2]=5.0f;
    matrix1[2][3]=4.0f;
    matrix1[3][0]=6.0f;
    matrix1[3][1]=-2.0f;
    matrix1[3][2]=0.0f;
    matrix1[3][3]=5.0f;

    float **res = ft_multiply_matrix(matrix, matrix1, 4,4);
     printf("===========================\n");
    float **inv = inverse_matrix(matrix1, 4);
    printf("===========================\n");
    float **a = ft_multiply_matrix(matrix1 , inv, 4,4);

    printf("===========================\n");
    for (int i=0; i<4; i++)
    {
        for (int j = 0; j < 4; j++)
            printf("matrix[%d][%d]==[%f] || ", i, j, (a[i][j]));
        printf("\n");            
    }

    if (ft_compare_matrix(a, matrix, 4,4))
        printf("the are equal\n");
    else
        printf("the are not equal\n");


    if (ft_compare_matrix(matrix, matrix1,2,2))
        printf("matrix and matrix1 are equal!!\n");
    else
        printf("matrix and matrix1 are !!! equal!!\n");
    printf("===========================\n");
    ft_transpose_matrix(&matrix1, 3, 3);
    printf("===========================\n");
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
            printf("matrix[%d][%d]==[%f] || ", i, j, matrix1[i][j]);
        printf("\n");
    }
    float **m =get_minor(matrix1, 0,0,3);
    printf("================MINOR===========\n");
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<2; j++)
            printf("matrix[%d][%d]==[%f] || ", i, j, m[i][j]);
        printf("\n");
    }
    printf ("determinant %f=======", determinant(matrix1, 3));
    float **inv = inverse_matrix(matrix1, 3);
    printf("===========================\n");
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
            printf("matrix[%d][%d]==[%f] || ", i, j, inv[i][j]);
        printf("\n");
    }
    t_point *p = ft_new_point(2,3,4);
    t_point *p2 = ft_new_point(-3,4,5);
    t_vector *v = ft_new_vector(-4, 6,8);
    // t_point *res = ft_transform(p, p2, -1);
    t_point *res = ft_scale(p, v, -1);

    // rotaion 
    float **m = rotate_x(PI / 4);
    float **m1 = rotate_x(PI / 2);
    t_point *p = ft_new_point(0,1,0);
    t_point *res = ft_multiply_matrix_vec(m, p);
    t_point *res1 = ft_multiply_matrix_vec(m1, p);
    // printf("Translation %f %f %f\n", res->x, res->y, res->z);
    printf("Translation %f %f %f\n", res->x, res->y, res->z);
    printf("Translation %f %f %f\n", res1->x, res1->y, res1->z);
*/

    int cor[6] = {0,0,0,0,0,1};
    t_point *p = ft_new_point(2,3,4);
    float **m = shearing_matrix(cor);
    t_point *res = ft_multiply_matrix_vec(m, p);
    printf("Translation %f %f %f\n", res->x, res->y, res->z);
    return 0;
}
// 00
// 0 \1 2 
// 3 \4 5
// 6 \7 8


// 0 3 6
// 1 4 7
// 2 5 8