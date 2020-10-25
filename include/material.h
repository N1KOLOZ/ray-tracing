#pragma once

#include "rt_utility.h"
#include "hittable.h"

struct hit_record;

class Material {
public:
    virtual bool Scatter(
            const ray& initial_r, const hit_record& rec, color& attenuation, ray& scattered_r
    ) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const color& a) : albedo(a) {}

    bool Scatter(
            const ray& initial_r, const hit_record& rec, color& attenuation, ray& scattered_r
    ) const override {
        vec3 scattered_dir = rec.normal + random_unit_vector();
        scattered_r = ray(rec.p, scattered_dir);
        attenuation = albedo;

        return true;
    }

public:
    color albedo;
};

class Metal : public Material {
public:
    Metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    bool Scatter(
            const ray& initial_r, const hit_record& rec, color& attenuation, ray& scattered_r
    ) const override {
        vec3 reflected_dir = reflect(unit_vector(initial_r.direction()), rec.normal);
        scattered_r = ray(rec.p, reflected_dir + fuzz*random_in_unit_sphere());
        attenuation = albedo;

        return (dot(scattered_r.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : n(index_of_refraction) {}

    bool Scatter(
            const ray& initial_r, const hit_record& rec, color& attenuation, ray& scattered_r
    ) const override {
        attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face ? 1 / n : n;

        vec3 u_in_dir = unit_vector(initial_r.direction()); // unit initial direction

        double cos_theta = fmin(dot(-u_in_dir, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool is_reflection = refraction_ratio * sin_theta > 1.0;

        vec3 scattered_dir;
        // reflectivity varies with an angle so the trick:
        if (is_reflection || reflectance(cos_theta, refraction_ratio) > random_double(0, 1.0))
            scattered_dir = reflect(u_in_dir, rec.normal);
        else
            scattered_dir = refract(u_in_dir, rec.normal, refraction_ratio);

        scattered_r = ray(rec.p, scattered_dir);
        return true;
    }

public:
    double n; // Index of Refraction

private:
    static double reflectance(double cos_theta, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cos_theta),5);
    }
};