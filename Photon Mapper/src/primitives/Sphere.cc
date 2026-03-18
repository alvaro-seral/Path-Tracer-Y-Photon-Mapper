/**
 * @file Sphere.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Sphere.hh"

#include <cmath>

Sphere::Sphere(const Point& center, double radius, const Material& material) :
    Primitive(material),
    center(center),
    radius(radius)
{
}

double Sphere::intersects(const Ray& ray) const
{
    double a = ray.direction * ray.direction;
    double b = 2 * ray.direction * (ray.origin - center);
    double c = (ray.origin - center) * (ray.origin - center) - radius * radius;
    double discriminant = b * b - 4 * a * c;

    if (!(discriminant < EPSILON)) {
        double t1 = (-b + std::sqrt(discriminant)) / (2 * a);
        double t2 = (-b - std::sqrt(discriminant)) / (2 * a);

        // Return closest intersection in the direction of the ray, otherwise
        // the closest in the other direction
        if (t1 > 0 && t2 > 0) return std::min(t1, t2);
        else if (t1 >= 0) return t1;
        else if (t2 >= 0) return t2;
        else return -std::min(std::abs(t1), std::abs(t2));
    }

    return INFINITY;
}

Direction Sphere::normalTo(const Point& point) const
{
    return (point - center).norm();
}