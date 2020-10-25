#include "include/rt_utility.h"

#include "include/material.h"
#include "include/color.h"
#include "include/sphere.h"
#include "include/hittable_list.h"
#include "include/camera.h"

#include <iostream>
#include <chrono>
#include <thread>

color Ray_color(const ray& r, const Hittable& world, int depth) {
    if (depth <= 0) {
        return color(0.0, 0.0, 0.0);
    }

    hit_record rec;
    if (world.Hit(r, 0.001, infinity, rec)) {
        ray scattered_r;
        color attenuation;
        if (rec.mat_ptr->Scatter(r, rec, attenuation, scattered_r))
            return attenuation * Ray_color(scattered_r, world, depth - 1);
        return color(0.0, 0.0, 0.0);
    }

    vec3 unit_dir = unit_vector(r.direction());
    auto t = 0.5*(unit_dir.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    // World
    Hittable_list world;

    auto material_ground = std::make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.5);
    auto material_right  = std::make_shared<Metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(std::make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(std::make_shared<Sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(std::make_shared<Sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
    world.add(std::make_shared<Sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    point3 lookfrom(3,3,2);
    point3 lookat(0,0,-1);
    vec3 vup(0,1,0);
    auto dist_to_focus = (lookfrom-lookat).length();
    auto aperture = 2.0;
    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);;


    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = camera.get_ray(u, v);
                pixel_color += Ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";

    return 0;
}