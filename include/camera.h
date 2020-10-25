#pragma once

#include "rt_utility.h"

class Camera {
public:
    Camera(
            point3 lookfrom,
            point3 lookat,
            vec3 v_up,
            double vertical_fov_deg, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focus_dist
    ) {
        double theta = degrees_to_radians(vertical_fov_deg);
        double h = tan(theta/2);
        double viewport_height = 2.0 * h;
        double viewport_width = aspect_ratio * viewport_height;

        // camera coordinate system
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(v_up, w));
        v = cross(w, u);


        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();

        return {origin + offset,lower_left_corner + s*horizontal + t*vertical - origin - offset};
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 w, u, v;
    double lens_radius;
};