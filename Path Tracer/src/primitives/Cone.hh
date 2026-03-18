/**
 * @file Cone.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Primitive.hh"
/// @brief A cone primitive.
class Cone : public Primitive {
public:
    /// @brief Base center of the cone.
    const Point     base_center;
    /// @brief Radius of the base of the cone.
    const double    base_radius;
    /// @brief Height of the cone.
    const double    height;
    /// @brief Direction of the cone's axis.
    const Direction direction;
    
    Cone(const Point& base_center, double base_radius, double height,
         const Direction& direction, const Material& material);

    virtual double intersects(const Ray& ray) const;

    virtual Direction normalTo(const Point& point) const;
};