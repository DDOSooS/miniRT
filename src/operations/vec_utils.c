# include "../../includes/minirt.h"

t_vector *ft_new_vector(float x, float y, float z)
{
    t_vector *new;

    new = malloc(sizeof(t_vector));
    if (!new)
        return (NULL);
    new->x = x;
    new->y = y;
    new->z = z;
    new->w = 0;
    return (new);
}

t_point *ft_new_point(float x, float y, float z)
{
    t_point *new;

    new = malloc(sizeof(t_point));
    if (!new)
        return (NULL);
    new->x = x;
    new->y = y;
    new->z = z;
    new->w = 1;
    return (new);
}

t_color *ft_new_color(float r, float g, float b)
{
    t_color *new;

    new = malloc(sizeof(t_color));
    if (!new)
        return (NULL);
    new->r = r;
    new->g = g;
    new->b = b;
    return (new);
}

t_color *ft_add_color(t_color *c1, t_color *c2)
{
    t_color *new;

    new = ft_new_color(0,0,0);
    if (!new)
        return (NULL);
    new->r = c1->r + c2->r;
    new->g = c1->g + c2->g;
    new->b = c1->b + c2->b;
    return (new);
}

t_color *ft_sub_color(t_color *c1, t_color *c2)
{
    t_color *new;

    new = ft_new_color(0,0,0);
    if (!new)
        return (NULL);
    new->r = c1->r - c2->r;
    new->g = c1->g - c2->g;
    new->b = c1->b - c2->b;
    return (new);
}

t_color *ft_scale_color(t_color *c, float scalar)
{
    t_color *new;

    new = ft_new_color(0,0,0);
    if (!new)
        return (NULL);
    new->r = c->r * scalar;
    new->g = c->g * scalar;
    new->b = c->b * scalar;
    return (new);
}

t_color *ft_multiply_color(t_color *c1, t_color *c2)
{
    t_color *new;

    new = ft_new_color(0,0,0);
    if (!new)
        return (NULL);
    new->r = c1->r * c2->r;
    new->g = c1->g * c2->g;
    new->b = c1->b * c2->b;
    return (new);
}


void    negate_vector(t_vector *vector)
{
    vector->x = -vector->x;
    vector->y = -vector->y;
    vector->z = -vector->z;
}

int compare_vector(float a, float b)
{
    if (fabs(a - b) < EPSILON)
        return (1);
    return (0);
}

float **ft_create_matrix(int n_rows, int n_cols)
{
    float  **matrix;
    
    matrix = (float **)malloc(n_rows * sizeof(float *));
    if (!matrix)
        return (NULL);
    for (int i = 0; i < n_rows; i++)
    {
        matrix[i] = (float *)malloc(n_cols * sizeof(float));
        if (!matrix[i])
        {
            for (int j = 0; j < i; j++)
                free(matrix[j]);
            free(matrix);
            return (NULL);
        }
        for (int j = 0; j < n_cols; j++)
            matrix[i][j] = 0;
    }
    return (matrix);
}

int ft_compare_matrix(float **m1, float **m2, int n_col, int n_row)
{
    int i;
    int j;
    i = -1;
    while(++i < n_row)
    {
        j = 0;
        --j;
        while(++j < n_col)
        {
            if (fabs(m1[i][j] - m2[i][j]) >= EPSILON)
                return (0);
        }
    }
    return (1);
}

float ** ft_multiply_matrix(float **m1, float **m2, int n_cols, int n_row)
{
    float **result;

    result = ft_create_matrix(n_row, n_cols);
    if (!result)
        return (NULL);
    for (int i = 0; i < n_row; i++)
    {
        for (int j = 0; j < n_cols; j++)
        {
            result[i][j] = 0;
            for (int k = 0; k < n_cols; k++)
                result[i][j] += m1[i][k] * m2[k][j];
        }
    }
    return (result);
}

t_vector *ft_multiply_matrix_vec(float **m, t_vector *v)
{
    t_vector *result;

    result = ft_new_vector(0,0,0);
    if (!result)
        return (NULL);
    result->x = m[0][0] * v->x + m[0][1] * v->y + m[0][2] * v->z + m[0][3] * v->w;
    result->y = m[1][0] * v->x + m[1][1] * v->y + m[1][2] * v->z + m[1][3] * v->w;
    result->z = m[2][0] * v->x + m[2][1] * v->y + m[2][2] * v->z + m[2][3] * v->w;
    result->w = m[3][0] * v->x + m[3][1] * v->y + m[3][2] * v->z + m[3][3] * v->w;
    // printf("=====================================\n");
    // printf("Translation %f %f %f\n", result->x, result->y, result->z);
    // printf("=====================================\n");
    return (result);
}

void ft_scale_matrix(float ***m, float scale,int n)
{
    t_vector *result;
    int i, j;
    result = ft_new_vector(0,0,0);
    if (!result)
        return ;
    for (int i = 0; i < n ; i++)
    {
        for (int j = 0; j < n; j++)
        {
            (*m)[i][j] *= scale;
        }
    }
}

void ft_transpose_matrix(float ***m, int n_rows, int n_col)
{
    float temp;
 
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < n_col; j++)
        {
            temp = (*m)[i][j];
            (*m)[i][j] = (*m)[j][i];
            (*m)[j][i] = temp;
        }
    }
}

float **ft_translate_matrix(t_point *point, int inverse)
{
    float **translation_matrix;

    translation_matrix = ft_create_matrix(4, 4);
    if (!translation_matrix)
        return (NULL);
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j && i < 3)
                translation_matrix[i][j] = 1;
            else 
                translation_matrix[i][j] = 0;
        }
    }
    translation_matrix[0][3] = inverse * point->x;
    translation_matrix[1][3] = inverse * point->y;
    translation_matrix[2][3] = inverse * point->z;
    translation_matrix[3][3] = inverse * point->w;
    return (translation_matrix);
}

void ft_free_matrix(float **m, int n)
{
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
    m = NULL;
}

t_point *ft_transform(t_point *p1 , t_point *p2, int inverse)
{
    t_point *transformed_point;
    float **matrix;

    matrix = ft_translate_matrix(p1, inverse);
    transformed_point = ft_multiply_matrix_vec(matrix, p2);
    ft_free_matrix(matrix, 4);
    return (transformed_point);
}


float **ft_scaling_matrix(t_point *scale, int inverse)
{
    float **scale_matrix;

    scale_matrix = ft_create_matrix(4, 4);
    if (!scale_matrix)
        return (NULL);
    for (int i = 0; i < 4; i++)
        for (int j; j < 4; i++)
            scale_matrix[i][j] = 0;
    scale_matrix[3][3] = 1;
    if (inverse == -1)
    {
        scale_matrix[0][0] = 1 / scale->x;
        scale_matrix[1][1] = 1 / scale->y;
        scale_matrix[2][2] = 1 / scale->z;
    }
    else
    {
        scale_matrix[1][1] = scale->y;
        scale_matrix[2][2] = scale->z;
        scale_matrix[0][0] = scale->x;
    }
    return (scale_matrix);
}

t_point *ft_scale(t_point *scale, t_point *point, int inverse)
{
    t_point *scaled_point;
    float **scale_matrix;

    scale_matrix = ft_scaling_matrix(scale, inverse);
    scaled_point = ft_multiply_matrix_vec(scale_matrix, point);
    // print_matrix(scale_matrix, 4, 4);
    ft_free_matrix(scale_matrix, 4);
    return (scaled_point);
}

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

float **get_minor(float **m, int row, int col, int n)
{
    int i, j;
    int mi, mj;
    float **minor;

    minor = ft_create_matrix(n - 1, n - 1);
    if (!minor)
        return (NULL);
    mi = 0;
    for (i = 0; i < n; i++)
    {
        if (i == row)
            continue;
        mj = 0;
        for (j = 0; j < n; j++)
        {
            if (j == col)
                continue;
            minor[mi][mj] = m[i][j];
            mj++;
        }
        mi++;
    }
    return (minor);
}

float radian( float degree)
{
    return (degree * PI / 180);
}

float **rotate_x(float rad)
{
    float **m;

    m = ft_create_matrix(4, 4);
    if (!m)
        return (NULL);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if ((i == 1 && j == 1) || (i == 2 && j == 2))
                m[i][j] = cos(rad);
            else if (i == 1 && j == 2)
                m[i][j] = -sin(rad);
            else if (i == 2 && j == 1)
                m[i][j] = sin(rad);
            else 
                m[i][j] = 0;    
        }
    m[0][0] = 1;
    m[3][3] = 1;
    return (m);
}

float **rotate_y(float rad)
{
    float **m;

    m = ft_create_matrix(4, 4);
    if (!m)
        return (NULL);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if ((i == 0 && j == 0) || (i == 2 && j == 2))
                m[i][j] = cos(rad);
            else if (i == 2 && j == 0)
                m[i][j] = -sin(rad);
            else if (i == 0 && j == 2)
                m[i][j] = sin(rad);
            else 
                m[i][j] = 0;    
        }
    m[1][1] = 1;
    m[3][3] = 1;
    return (m);
}

float **rotate_z(float rad)
{
    float **m;

    m = ft_create_matrix(4, 4);
    if (!m)
        return (NULL);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if ((i == 1 && j == 1) || (i == 0 && j == 0))
                m[i][j] = cos(rad);
            else if (i == 0 && j == 1)
                m[i][j] = -sin(rad);
            else if (i == 1 && j == 1)
                m[i][j] = sin(rad);
            else 
                m[i][j] = 0;    
        }
    m[2][2] = 1;
    m[3][3] = 1;
    return (m);
}

float **shearing_matrix(int *coord)
{
    float **shearing_matrix;

    shearing_matrix = ft_create_matrix(4, 4);
    if (!shearing_matrix)
        return (NULL);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                shearing_matrix[i][j] = 1;
            else 
                shearing_matrix[i][j] = 0;    
        }
    shearing_matrix[0][1] = coord[0];
    shearing_matrix[0][2] = coord[1];
    shearing_matrix[1][0] = coord[2];
    shearing_matrix[1][2] = coord[3];
    shearing_matrix[2][0] = coord[4];
    shearing_matrix[2][1] = coord[5];
    return (shearing_matrix);
}


float determinant(float **m, int n)
{
    float det;
    int sign;
    float **minor;

    if (n == 2)
        return (m[0][0] * m[1][1] - m[0][1] * m[1][0]);
    det = 0;
    sign = 1;
    for (int i = 0; i < n; i++)
    {
        minor = get_minor(m, 0, i, n);
        if (!minor)
            return (0);
        det += sign * m[0][i] * determinant(minor, n - 1);
        sign = -sign;
        free(minor);
    }
    return (det);
}

float **inverse_matrix(float **m, int n)
{
    float **inverse;
    float **minor;
    float det;
    int i, j, sign;

    det = determinant(m, n);
    if (det == 0)
        return (NULL);
    inverse = ft_create_matrix(n, n);
    if (!inverse)
        return (NULL);
    for (i = 1; i <= n; i++)
    {
        for (j = 1; j <= n; j++)
        {
            minor = get_minor(m, i-1, j-1, n);
            if (!minor)
                return (NULL);
            inverse[j-1][i-1] = pow(-1, i + j) * determinant(minor, n - 1);
            free(minor);
        }
    }
    ft_transpose_matrix(&inverse, n, n);
    ft_scale_matrix(&inverse, (float)(1.0f / det), n);
    return (inverse);
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

t_vector *vector_normilze(t_vector *vec)
{
    if (!vec) 
        return NULL;

    float magnitude = vec_lenght(vec);
    if (magnitude == 0) 
        return ft_new_vector(0, 0, 0);

    t_vector *new = ft_new_vector(0, 0, 0);
    if (!new)
        return NULL;

    new->x = vec->x / magnitude;
    new->y = vec->y / magnitude;
    new->z = vec->z / magnitude;

    return new;
}