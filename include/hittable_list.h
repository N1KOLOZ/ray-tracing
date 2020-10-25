#pragma once

#include "hittable.h"

#include <memory>
#include <utility>
#include <vector>

class Hittable_list : public Hittable {
public:
    Hittable_list() {}
    Hittable_list(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(std::shared_ptr<Hittable> object) { objects.push_back(object); }

    bool Hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    std::vector<std::shared_ptr<Hittable>> objects;
};

bool Hittable_list::Hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool is_hit = false;
    auto closest = t_max;

    for (const auto& object : objects) {
        if (object->Hit(r, t_min, closest, temp_rec)) {
            is_hit = true;
            closest = temp_rec.t;
        }
    }

    rec = temp_rec;

    return is_hit;
}