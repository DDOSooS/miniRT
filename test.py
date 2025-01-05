import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d.art3d import Line3DCollection

# Define a sphere centered at the origin
def sphere_at_origin(radius, num_points):
    phi = np.linspace(0, np.pi, num_points)  # Polar angle
    theta = np.linspace(0, 2 * np.pi, num_points)  # Azimuthal angle
    phi, theta = np.meshgrid(phi, theta)
    
    x = radius * np.sin(phi) * np.cos(theta)
    y = radius * np.sin(phi) * np.sin(theta)
    z = radius * np.cos(phi)
    
    return x, y, z

# Translate the sphere
def translate_sphere(x, y, z, tx, ty, tz):
    return x + tx, y + ty, z + tz

# Plot the sphere and its normals
def plot_sphere_with_normals(x, y, z, title):
    fig = plt.figure(figsize=(10, 5))
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot the sphere
    ax.plot_surface(x, y, z, color='b', alpha=0.5)
    
    # Plot normals
    for i in range(0, x.shape[0], 5):  # Skip points for clarity
        for j in range(0, x.shape[1], 5):
            point = np.array([x[i, j], y[i, j], z[i, j]])
            normal = point / np.linalg.norm(point)  # Normal vector
            ax.quiver(*point, *normal, length=0.2, color='r')  # Plot normal
    
    ax.set_title(title)
    ax.set_xlabel('X')
    ax.set_ylabel('Y')
    ax.set_zlabel('Z')
    plt.show()

# Parameters
radius = 1
num_points = 20
translation = (2, 0, 0)  # Translate sphere along the x-axis

# Sphere at origin
x, y, z = sphere_at_origin(radius, num_points)
plot_sphere_with_normals(x, y, z, "Sphere at Origin with Normals")

# Translated sphere
x_t, y_t, z_t = translate_sphere(x, y, z, *translation)
plot_sphere_with_normals(x_t, y_t, z_t, "Translated Sphere with Normals")