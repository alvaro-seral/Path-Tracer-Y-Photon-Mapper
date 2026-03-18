/**
 * @file Point.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Coordinate.hh"
#include "math_structures/Vec3.hh"

/// @brief A point in space
class Point : public Coordinate {
public:
    /**
     * @brief Construct a new Point object
     *
     * @param x x component
     * @param y y component
     * @param z z component
     */
    Point(double x, double y, double z);

    /**
     * @brief Construct a new Point object
     *
     * @param v 3D vector
     */
    Point(const Vec3& v);
};