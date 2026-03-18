/**
 * @file Direction.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Coordinate.hh"
#include "math_structures/Vec3.hh"

/// @brief A direction in space
class Direction : public Coordinate {

public:
    /**
     * @brief Construct a new Direction object
     *
     * @param x x component
     * @param y y component
     * @param z z component
     */
    Direction(double x, double y, double z);

    /**
     * @brief Construct a new Direction object
     *
     * @param v 3D vector
     */
    Direction(const Vec3& v);
};