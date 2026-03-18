/**
 * @file Coordinate.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Mat4.hh"
#include "math_structures/Vec3.hh"

/// @brief A homogenous coordinate
class Coordinate : public Vec3 {

public:
    /// @brief Homogenous coordinate
    double w = 0;

    /**
     * @brief Construct a new Coordinate object
     *
     * @param x x component
     * @param y y component
     * @param z z component
     * @param w w (homogenous) component
     */
    Coordinate(double x, double y, double z, double w);

    /**
     * @brief Construct a new Coordinate object
     *
     * @param v 3D vector
     * @param w w (homogenous) component
     */
    Coordinate(const Vec3& v, double w);

    /**
     * @brief Construct a new Coordinate object
     *
     * @param c Coordinate
     */
    Coordinate(const Coordinate& c);

    /**
     * @brief Assign the values of another coordinate to the coordinate
     *
     * @param c Coordinate
     * @return Coordinate
     */
    Coordinate operator=(const Coordinate& c);

    /**
     * @brief Multiply a 4-dimensional matrix by a homogenous coordinate
     *
     * @param m 4x4 Matrix
     * @param c Homogenous coordinate
     * @return Coordinate
     */
    friend Coordinate operator*(const Mat4& m, const Coordinate& c);
};