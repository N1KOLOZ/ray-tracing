#pragma once

#include "rt_utility.h"

#include <cmath>
#include <iostream>

class vec3 {
public:
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& other) {
        e[0] += other.e[0];
        e[1] += other.e[1];
        e[2] += other.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min ,max));
    }

public:
    double e[3];
};

// Type aliases for vec3
using point3 = vec3; // 3D point
using color = vec3; // RGB color

// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream& os, const vec3& rhs) {
    return os << rhs.e[0] << ' ' << rhs.e[1] << ' ' << rhs.e[2];
}

inline vec3 operator+(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs.e[0] + rhs.e[0], lhs.e[1] + rhs.e[1], lhs.e[2] + rhs.e[2]);
}

inline vec3 operator-(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs.e[0] - rhs.e[0], lhs.e[1] - rhs.e[1], lhs.e[2] - rhs.e[2]);
}

inline vec3 operator*(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs.e[0] * rhs.e[0], lhs.e[1] * rhs.e[1], lhs.e[2] * rhs.e[2]);
}

inline vec3 operator*(double t, const vec3& rhs) {
    return vec3(t*rhs.e[0], t*rhs.e[1], t*rhs.e[2]);
}

inline vec3 operator*(const vec3& lhs, double t) {
    return t * lhs;
}

inline vec3 operator/(const vec3& lhs, double t) {
    return (1/t) * lhs;
}

inline double dot(const vec3& lhs, const vec3& rhs) {
    return lhs.e[0] * rhs.e[0]
         + lhs.e[1] * rhs.e[1]
         + lhs.e[2] * rhs.e[2];
}

inline vec3 cross(const vec3& lhs, const vec3& rhs) {
    return vec3(lhs.e[1] * rhs.e[2] - lhs.e[2] * rhs.e[1],
                lhs.e[2] * rhs.e[0] - lhs.e[0] * rhs.e[2],
                lhs.e[0] * rhs.e[1] - lhs.e[1] * rhs.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

vec3 random_unit_vector() {
    double phi = random_double(0.0, 2*PI);
    double z = random_double(-1.0, 1.0);
    double r = sqrt(1 - z*z);
    return vec3(r*cos(phi), r*sin(phi), z);
}

vec3 random_in_unit_sphere() {
    vec3 result = random_double() * random_unit_vector();
    return result;
}

vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

vec3 reflect(const vec3& in_v, const vec3& n) {
    return in_v - 2*dot(in_v,n)*n;
}

// ref_idx = n1 / n2, where n1, n2 are refractive indices
vec3 refract(const vec3& u_in_v, const vec3& n, double ref_idx) {
    double cos_theta = dot(-u_in_v, n);
    vec3 r_ref_perp = ref_idx * (u_in_v + cos_theta * n);
    vec3 r_ref_parallel = -sqrt(fabs(1.0 - r_ref_perp.length_squared())) * n;
    return r_ref_perp + r_ref_parallel;
}