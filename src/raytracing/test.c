s_camera *new_camera(float h_size, float w_size, float fov, t_point p, t_vector dir)
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
    camera->fov = fov; 
    aspect = camera->w_size / camera->h_size;
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
    return camera;
}