/**
 * @file Plane.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Primitive.hh"

/// @brief A plane primitive.
class Plane : public Primitive {
public:
    /// @brief Distance from the origin to the plane along its normal.
    const double    distance;
    /// @brief Normal vector of the plane.
    const Direction normal;

    Plane(double distance, const Direction& normal, const Material& material);

    virtual double intersects(const Ray& ray) const;

    virtual Direction normalTo(const Point& point) const;
};