# cao_parallel_ray_tracer

# Parallel Ray Tracer with OpenMP

This project is a simple ray tracer implemented in C++ and parallelized using OpenMP. It renders a 3D scene with basic geometric shapes and lighting. The primary goal of this project is to demonstrate the performance benefits of parallelizing computationally intensive tasks like ray tracing.

## Features

-   Renders a 3D scene with spheres.
-   Basic Phong shading model for lighting.
-   Parallelized rendering loop using OpenMP for significant speedup.
-   Outputs the final rendered image as a PPM file.

## Technologies Used

-   C++
-   OpenMP

## Getting Started

### Prerequisites

-   A C++ compiler that supports OpenMP (e.g., GCC, Clang).

### Compilation

To compile the project, you can use the provided `Makefile`. Simply run the `make` command in the project's root directory:

