/**
 * @file Cylinder.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Cylinder.hh"

#include <cmath>
#include <cassert>
#include <vector>

Cylinder::Cylinder(const Point& base_center, double base_radius, double height,
                   const Direction& direction, const Material& material) :
    Primitive(material),
    base_center(base_center),
    base_radius(base_radius),
    height(height),
    direction(direction.norm())
{
    assert(base_radius > 0);
}

// https://mrl.cs.nyu.edu/~dzorin/rend05/lecture2.pdf
double Cylinder::intersects(const Ray& ray) const 
{
    Direction delta = ray.origin - base_center; // axis direction

    double a = (ray.direction - ray.direction * direction * direction)
               * (ray.direction - ray.direction * direction * direction);
               
    double b = ((ray.direction - ray.direction * direction * direction)
               * (delta - delta * direction * direction)) * 2;
    
    double c = (delta - delta * direction * direction)
               * (delta - delta * direction * direction) 
               - base_radius * base_radius;

    // Candidatos de intersecciones
    std::vector<double> t_candidates;
    t_candidates.push_back(INFINITY);

    // Calcular la intersección con el lateral del cilindro
    double radicand = b * b - 4 * a * c;
    if (radicand >= EPSILON) {
        double square = std::sqrt(radicand);
        double t1 = (-b + square) / (2 * a);
        double t2 = (-b - square) / (2 * a);

            Point i1 = ray.origin + ray.direction * t1;
        if ((0 <= (i1 - base_center) * direction) && ((i1 - base_center) * direction <= height)) {
            t_candidates.push_back(t1);
        }

        Point i2 = ray.origin + ray.direction * t2;
        if ((0 <= (i2 - base_center) * direction) && ((i2 - base_center) * direction <= height)) {
            t_candidates.push_back(t2);
        }
    }

    // Calcular la intersección con las tapas inferior y superior del cilindro
    double denominator = ray.direction * direction;

    if (std::abs(denominator) > EPSILON) {
        // Centro de la tapa inferior
        Point c3 = base_center; 
        // Distancia del origen del rayo a la que se encuentra el punto 
        // donde el rayo intersecta el plano de la tapa inferior
        double t3 = (c3 - ray.origin) * direction / denominator; 
        // Punto de intersección con el plano de la tapa inferior
        Point i3 = ray.origin + ray.direction * t3;
        // Verificar si la intersección está dentro del radio de la base
        if ((i3 - c3).mod() <= base_radius) {
            t_candidates.push_back(t3);
        }
    }

    if (std::abs(denominator) > EPSILON) {
        // Centro de la tapa superior
        Point c4 = base_center + direction * height;
        // Distancia del origen del rayo a la que se encuentra el punto 
        // donde el rayo intersecta el plano de la tapa superior
        double t4 = (c4 - ray.origin) * direction / denominator;
        // Punto de intersección con el plano de la tapa superior
        Point i4 = ray.origin + ray.direction * t4; 
        // Verificar si la intersección está dentro del radio de la base
        if ((i4 - c4).mod() <= base_radius) {
            t_candidates.push_back(t4);
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

Direction Cylinder::normalTo(const Point& point) const
{
    double delta = (point - base_center) * direction;

    // En la base inferior
    if (delta <= EPSILON)
        return -direction;

    // En la base superior
    if (delta >= height - EPSILON)
        return direction;

    // En el  lateral
    // Restar la componente a lo largo del eje para obtener la dirección radial
    Point axis_point = base_center + direction * delta;
    Direction radial = point - axis_point;
    return radial.norm();
}
