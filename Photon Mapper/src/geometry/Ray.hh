/**
 * @file Ray.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Direction.hh"
#include "Point.hh"

/// @brief A ray with an origin and a direction
class Ray {
public:
    /// @brief Point of origin of the ray
    Point origin;

    /// @brief Direction of the ray
    Direction direction;

    Ray() = default;

    /**
     * @brief Construct a new Ray object
     *
     * @param origin Point of origin of the ray
     * @param direction Direction of the ray
     */
    Ray(const Point& origin, const Direction& direction);
};