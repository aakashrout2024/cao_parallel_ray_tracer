#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>

const double INF = std::numeric_limits<double>::infinity();

struct Vec3 {
    double x, y, z;
    Vec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
    Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
    Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
    double dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vec3 cross(const Vec3& other) const { return Vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x); }
    Vec3& normalize() {
        double len = std::sqrt(x * x + y * y + z * z);
        if (len > 0) {
            double invLen = 1 / len;
            x *= invLen;
            y *= invLen;
            z *= invLen;
        }
        return *this;
    }
};

struct Ray {
    Vec3 origin, direction;
    Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}
};

struct Sphere {
    Vec3 center;
    double radius;
    Vec3 color;

    Sphere(const Vec3& center, double radius, const Vec3& color) : center(center), radius(radius), color(color) {}

    bool intersect(const Ray& ray, double& t) const {
        Vec3 oc = ray.origin - center;
        double a = ray.direction.dot(ray.direction);
        double b = 2.0 * oc.dot(ray.direction);
        double c = oc.dot(oc) - radius * radius;
        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) {
            return false;
        } else {
            double t1 = (-b - std::sqrt(discriminant)) / (2.0 * a);
            double t2 = (-b + std::sqrt(discriminant)) / (2.0 * a);
            t = (t1 > 0.001) ? t1 : t2;
            return t > 0.001;
        }
    }
};

Vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, int depth) {
    if (depth <= 0) {
        return Vec3(0, 0, 0);
    }

    double t_closest = INF;
    const Sphere* hit_sphere = nullptr;

    for (const auto& sphere : spheres) {
        double t = 0;
        if (sphere.intersect(ray, t) && t < t_closest) {
            t_closest = t;
            hit_sphere = &sphere;
        }
    }

    if (!hit_sphere) {
        return Vec3(0.7, 0.7, 0.9); // Background color
    }

    Vec3 hit_point = ray.origin + ray.direction * t_closest;
    Vec3 normal = (hit_point - hit_sphere->center).normalize();
    
    // Simple reflection
    Vec3 reflect_dir = ray.direction - normal * 2 * ray.direction.dot(normal);
    return hit_sphere->color * 0.2 + trace(Ray(hit_point, reflect_dir), spheres, depth - 1) * 0.8;
}

void render(const std::vector<Sphere>& spheres) {
    const int width = 1920;
    const int height = 1080;
    const double fov = 3.1415926535 / 2.0;
    std::vector<Vec3> image(width * height);

    #pragma omp parallel for schedule(dynamic)
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            double x = (2 * (i + 0.5) / (double)width - 1) * tan(fov / 2.) * width / (double)height;
            double y = -(2 * (j + 0.5) / (double)height - 1) * tan(fov / 2.);
            Vec3 dir = Vec3(x, y, -1).normalize();
            image[i + j * width] = trace(Ray(Vec3(0, 0, 0), dir), spheres, 4);
        }
    }

    std::ofstream ofs("./output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i) {
        unsigned char r = (unsigned char)(std::min(1.0, image[i].x) * 255);
        unsigned char g = (unsigned char)(std::min(1.0, image[i].y) * 255);
        unsigned char b = (unsigned char)(std::min(1.0, image[i].z) * 255);
        ofs << r << g << b;
    }
    ofs.close();
}

int main() {
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3(-3, 0, -16), 2, Vec3(1, 0.32, 0.36)));
    spheres.push_back(Sphere(Vec3(-1, -1.5, -12), 2, Vec3(0.9, 0.76, 0.46)));
    spheres.push_back(Sphere(Vec3(1.5, -0.5, -18), 3, Vec3(0.65, 0.77, 0.97)));
    spheres.push_back(Sphere(Vec3(7, 5, -18), 4, Vec3(0.90, 0.90, 0.90)));
    
    double start_time = omp_get_wtime();
    render(spheres);
    double end_time = omp_get_wtime();
    
    std::cout << "Rendering complete. Time taken: " << end_time - start_time << " seconds." << std::endl;

    return 0;
}
