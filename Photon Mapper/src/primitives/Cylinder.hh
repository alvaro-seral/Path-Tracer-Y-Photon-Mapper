/**
 * @file Cylinder.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Primitive.hh"
/// @brief A cylinder primitive.
class Cylinder : public Primitive {
public:
    const Point     base_center;
    const double    base_radius;
    const double    height;
    const Direction direction;
    
    Cylinder(const Point& base_center, double base_radius, double height,
             const Direction& direction, const Material& material);

    virtual double intersects(const Ray& ray) const;

    virtual Direction normalTo(const Point& point) const;
};

