/**
 * @file Rotation.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */
#pragma once

#include "RotX.hh"
#include "RotY.hh"
#include "RotZ.hh"

/// @brief A rotation matrix.
class Rotation : public Mat4 {

private:
    /// @brief X axis rotation
    RotX x;

    /// @brief Y axis rotation
    RotY y;

    /// @brief Z axis rotation
    RotZ z;

public:
    /**
     * @brief Construct a new Rotation object
     *
     * @param theta_x X axis rotation
     * @param theta_y Y axis rotation
     * @param theta_z Z axis rotation
     */
    Rotation(double theta_x, double theta_y, double theta_z);

    virtual Mat4 inv() const override;
};
