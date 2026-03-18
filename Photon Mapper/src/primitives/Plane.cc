/**
 * @file Plane.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Plane.hh"

#include <cmath>

Plane::Plane(double distance, const Direction& normal,
             const Material& material) :
    Primitive(material),
    distance(distance),
    normal(normal.norm())
{
}

double Plane::intersects(const Ray& ray) const
{
    const double numerator = (distance + ray.origin * normal);
    const double denominator = ray.direction * normal;

    if (!(std::abs(denominator) < EPSILON)) return -numerator / denominator;
    else return INFINITY;
}

Direction Plane::normalTo([[maybe_unused]] const Point& point) const
{
    return normal;
}
