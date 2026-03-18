/**
 * @file Translation.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Mat4.hh"

/// @brief A translation matrix.
class Translation : public Mat4 {
public:
    /**
     * @brief Construct a new Translation object
     *
     * @param x x translation
     * @param y y translation
     * @param z z translation
     */
    Translation(double x, double y, double z);

    virtual Mat4 inv() const override;
};