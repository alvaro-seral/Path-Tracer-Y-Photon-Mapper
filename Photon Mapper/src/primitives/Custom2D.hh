/**
 * @file Custom2D.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Primitive.hh"

#include <vector>

/// @brief A custom 2D primitive. A sprite build from a series of ordened points in the same plane surface
class Custom2D : public Primitive {
public:
    /// @brief Points that form the custom 2D primitive
    std::vector<Point> points;

    /// @brief Normal to the plane of the custom 2D primitive
    const Direction normal;

    Custom2D(const std::vector<Point>& points, const Direction& normal, 
             const Material& material);

    virtual double intersects(const Ray& ray) const;

    virtual Direction normalTo(const Point& point) const;
};
