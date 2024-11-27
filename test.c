#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 1000
#define HEIGHT 500
#define D 500
#define BACKGROUND_COLOR 50

// Vector structure
typedef struct {
    double x;
    double y;
    double z;
} Vector;

// Color structure
typedef struct {
    unsigned char r;
    unsigned char g; 
    unsigned char b;
} Color;

// Image structure
typedef struct {
    Color **pixels;
    int width;
    int height;
} Image;

// Vector operations
Vector create_vector(double x, double y, double z) {
    Vector v = {x, y, z};
    return v;
}

double dot_product(Vector u, Vector v) {
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

double vector_norm(Vector v) {
    return sqrt(dot_product(v, v));
}

Vector normalize_vector(Vector v) {
    double norm = vector_norm(v);
    Vector result = {v.x / norm, v.y / norm, v.z / norm};
    return result;
}

Vector direction(int x, int y) {
    Vector dir = create_vector(
        x - WIDTH / 2.0,
        y - HEIGHT / 2.0,
        D
    );
    return normalize_vector(dir);
}

// Intersection calculation
double intersection(Vector u, Vector center, double radius) {
    double a = 1;  // Since u is normalized
    Vector OC = center;  // Center relative to origin
    double b = -2 * dot_product(u, OC);
    double c = dot_product(OC, OC) - radius * radius;
    
    double delta = b * b - 4 * a * c;
    
    if (delta == 0) {
        double t = -b / (2 * a);
        return t > 0 ? t : 0;
    }
    else if (delta > 0) {
        double t1 = (-b - sqrt(delta)) / (2 * a);
        double t2 = (-b + sqrt(delta)) / (2 * a);
        
        if (t1 >= 0 && t2 >= 0) {
            return fmin(t1, t2);
        }
        return fmax(0, fmax(t1, t2));
    }
    return 0;
}

// Image operations
Image* create_image(int width, int height) {
    Image* img = malloc(sizeof(Image));
    img->width = width;
    img->height = height;
    
    img->pixels = malloc(height * sizeof(Color*));
    for (int i = 0; i < height; i++) {
        img->pixels[i] = malloc(width * sizeof(Color));
    }
    
    // Fill with background color
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            img->pixels[y][x].r = BACKGROUND_COLOR;
            img->pixels[y][x].g = BACKGROUND_COLOR;
            img->pixels[y][x].b = BACKGROUND_COLOR;
        }
    }
    
    return img;
}

void free_image(Image* img) {
    for (int i = 0; i < img->height; i++) {
        free(img->pixels[i]);
    }
    free(img->pixels);
    free(img);
}

void save_image(Image* img, const char* filename) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error: Cannot open file for writing\n");
        return;
    }
    
    // Write PPM header
    fprintf(fp, "P6\n%d %d\n255\n", img->width, img->height);
    
    // Write pixel data
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            fwrite(&img->pixels[y][x], sizeof(Color), 1, fp);
        }
    }
    
    fclose(fp);
}

void render_sphere(Image* img, Vector center, double radius, Color sphere_color) {
    for (int x = 0; x < img->width; x++) {
        for (int y = 0; y < img->height; y++) {
            Vector ray_dir = direction(x, y);
            
            if (intersection(ray_dir, center, radius) > 0) {
                img->pixels[y][x] = sphere_color;
            }
        }
    }
}

int main() {
    // Create image
    Image* img = create_image(WIDTH, HEIGHT);
    
    // Define sphere properties
    Vector center = create_vector(0, 0, 10);
    double radius = 1.0;
    Color sphere_color = {58, 84, 216};  // Blue color
    
    // Render sphere
    render_sphere(img, center, radius, sphere_color);
    
    // Save result
    save_image(img, "sphere12.ppm");
    
    // Cleanup
    free_image(img);
    
    printf("Image has been saved as 'sphere.ppm'\n");
    return 0;
}