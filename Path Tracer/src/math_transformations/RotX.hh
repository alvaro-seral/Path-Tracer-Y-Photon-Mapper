/**
 * @file RotX.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Mat4.hh"

/// @brief A rotation around the x-axis.
class RotX : public Mat4 {
public:
    /**
     * @brief Construct a new Rot X object
     *
     * @param theta Angle of rotation (in radians)
     */
    RotX(double theta);

    virtual Mat4 inv() const override;
};