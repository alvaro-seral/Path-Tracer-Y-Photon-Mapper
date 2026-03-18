/**
 * @file ChangeOfBasis.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Mat4.hh"
#include "math_structures/Vec3.hh"

#include "geometry/Direction.hh"
#include "geometry/Point.hh"

/// @brief A change of basis matrix.
class ChangeOfBasis : public Mat4 {
public:
    /**
     * @brief Construct a new Change Of Basis object
     *
     * @param origin Origin point
     * @param x x direction
     * @param y y direction
     * @param z z direction
     */
    ChangeOfBasis(Point origin, Direction x, Direction y, Direction z);
};