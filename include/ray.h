#pragma once

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3& direction)
            : _origin(origin), _dir(direction)
    {}

    point3 origin() const  { return _origin; }
    vec3 direction() const { return _dir; }

    point3 at(double t) const {
        return _origin + t * _dir;
    }

public:
    point3 _origin;
    vec3 _dir;
};