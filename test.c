#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Function to check if a point is on the sphere
bool is_point_on_sphere(float x, float y, float z, float h, float k, float l, float radius) {
    // Calculate the squared distance from the point to the sphere's center
    float distance_squared = (x - h) * (x - h) + (y - k) * (y - k) + (z - l) * (z - l);
    float radius_squared = radius * radius;
    
    // Check if the distance squared is approximately equal to the radius squared
    float tolerance = 1e-5; // Small tolerance for floating-point comparison
    return distance_squared == radius_squared;
}

int main() {
    // Example point and sphere properties
    float x = 3.00000100, y = 4.0, z = 0; // Point to check
    float h = 0.0, k = 0.0, l = 0.0; // Center of the sphere
    float radius = 5.0;              // Radius of the sphere

    // Check if the point is on the sphere
    if (is_point_on_sphere(x, y, z, h, k, l, radius)) {
        printf("The point (%.2f, %.2f, %.2f) is on the sphere.\n", x, y, z);
    } else {
        printf("The point (%.2f, %.2f, %.2f) is NOT on the sphere.\n", x, y, z);
    }

    return 0;
}
