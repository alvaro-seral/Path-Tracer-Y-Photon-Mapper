/**
 * @file Cone.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Cone.hh"

#include <cmath>
#include <cassert>
#include <vector>

Cone::Cone(const Point& base_center, double base_radius, double height,
           const Direction& direction, const Material& material) :
    Primitive(material),
    base_center(base_center),
    base_radius(base_radius),
    height(height),
    direction(direction.norm())
{
    assert(base_radius > 0);
}

double Cone::intersects(const Ray& ray) const 
{
    Point apex = base_center - direction * height;
    Direction delta = ray.origin - apex; // axis direction
    double cos_theta = height / std::sqrt(height * height + base_radius * base_radius);
    double cos2_theta = cos_theta * cos_theta;

    double a = (ray.direction * direction) * (ray.direction * direction) 
               - cos2_theta;

    double b = ((ray.direction * direction) * (delta * direction) 
               - (ray.direction * delta) * cos2_theta) * 2;

    double c = (delta * direction) * (delta * direction) 
               - (delta * delta) * cos2_theta;

    // Candidatos de intersecciones
    std::vector<double> t_candidates;
    t_candidates.push_back(INFINITY);

    // Calcular la intersección con el lateral del cono
    double radicand = b * b - 4 * a * c;
    if (radicand >= EPSILON) {
        double square = std::sqrt(radicand);
        double t1 = (-b + square) / (2 * a);
        double t2 = (-b - square) / (2 * a);

        Point i1 = ray.origin + ray.direction * t1;
        if ((0 <= (i1 - apex) * direction) && ((i1 - apex) * direction <= height)) {
            t_candidates.push_back(t1);
        }

        Point i2 = ray.origin + ray.direction * t2;
        if ((0 <= (i2 - apex) * direction) && ((i2 - apex) * direction <= height)) {
            t_candidates.push_back(t2);
        }
    }

    // Calcular la intersección con la base del cono
    double denominator = ray.direction * direction;

    if (std::abs(ray.direction * direction) > EPSILON) {
        Point c3 = base_center;
        double t3 = (c3 - ray.origin) * direction / denominator;
        Point i3 = ray.origin + ray.direction * t3; // intersection basis
        if ((i3 - c3).mod() <= base_radius) {
            t_candidates.push_back(t3);
        }
    }

    // Elegir la intersección más cercana válida
    double min_positive = INFINITY;
    double min_negative = -INFINITY;
    for (double t : t_candidates) {
        if (t >= 0 && t < min_positive) {
            min_positive = t;
        }
        else if (t < 0 && t > min_negative) {
            min_negative = t;
        }
    }

    if (min_positive < INFINITY) {
        return min_positive;
    }
    else if (min_negative > -INFINITY) {
        return min_negative;
    }
    else return INFINITY;
}

Direction Cone::normalTo(const Point& point) const 
{
    Direction inverse_direction = -direction;
    double delta = (point - base_center) * inverse_direction;

    // En la base inferior
    if (delta <= EPSILON) {
        return -inverse_direction;
    }

    // En el lateral
    Point axis_point = base_center + inverse_direction * delta;
    Direction radial = point - axis_point;
    Direction normal = radial.norm() * height - inverse_direction * base_radius;
    return normal.norm();
}
