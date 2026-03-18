/**
 * @file Sphere.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Primitive.hh"

/// @brief A sphere primitive.
class Sphere : public Primitive {
public:
    /// @brief Center point
    const Point center;
    /// @brief Radius of sphere
    const double radius;

    Sphere(const Point& center, double radius, const Material& material);

    virtual double intersects(const Ray& ray) const;

    virtual Direction normalTo(const Point& point) const;
};