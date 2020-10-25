#pragma once

#include <utility>

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
    Sphere() {}

    Sphere(point3 center, double radius, std::shared_ptr<Material> m)
        : center(center),
          radius(radius),
          mat_ptr(m) {}

    bool Hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 center;
    double radius;
    std::shared_ptr<Material> mat_ptr;
};

bool Sphere::Hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;

    // a*t^2 + b*t + c = 0
    double a = r.direction().length_squared();
    double half_b = dot(oc, r.direction());
    double c = oc.length_squared() - radius*radius;
    double discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        double root = sqrt(discriminant);

        double t = (-half_b - root) / a;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        t = (-half_b + root) / a;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}