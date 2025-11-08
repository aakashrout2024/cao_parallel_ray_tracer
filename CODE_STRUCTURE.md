# Code Structure

This document outlines the structure of the C++ code for the parallel ray tracer.

## `main.cpp`

This is the main file containing the entire ray tracing logic. It is structured as follows:

-   **Vector Math (`Vec3` struct)**: A simple 3D vector class with basic operations like addition, subtraction, dot product, and cross product. This is used for representing points, directions, and colors.

-   **Ray (`Ray` struct)**: Represents a ray with an origin point and a direction vector.

-   **Sphere (`Sphere` struct)**: Defines a sphere with a center, radius, and material properties (color). Includes a method `intersect()` to check for intersections with a ray.

-   **`trace()` function**: This is the core recursive function of the ray tracer. Given a ray, it finds the closest intersecting object and returns its color. It handles reflections by recursively calling itself.

-   **`render()` function**: This function contains the main rendering loop that iterates over each pixel of the image.
    -   It generates a primary ray for each pixel.
    -   It calls the `trace()` function to get the color for that ray.
    -   **Parallelization**: The outer loop of this function is parallelized using the `#pragma omp parallel for` directive. This is where OpenMP distributes the work across multiple threads.

-   **`main()` function**:
    -   Initializes the scene by creating `Sphere` objects.
    -   Calls the `render()` function to generate the image data.
    -   Saves the final image to a PPM file (`output.ppm`).

## `Makefile`

A simple `Makefile` is provided for easy compilation of the project. It includes the necessary flags to enable OpenMP support (`-fopenmp`).

