/**
 * @file Scene.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Scene.hh"

#include <cmath>

Scene::Scene(const std::vector<std::shared_ptr<Primitive>>&  objects = {},
             const std::vector<std::shared_ptr<PointLight>>& lights = {}) :
    objects(objects),
    lights(lights)
{
}

std::pair<double, std::shared_ptr<Primitive>>
Scene::closest_intersection(const Ray& ray) const
{
    double                     distance = INFINITY;
    std::shared_ptr<Primitive> closest_object = nullptr;

    for (std::shared_ptr<Primitive> object : this->objects) {
        double d = object->intersects(ray);

        if (d < distance && d > 0) {
            distance = d;
            closest_object = object;
        }
    }

    return make_pair(distance, closest_object);
}