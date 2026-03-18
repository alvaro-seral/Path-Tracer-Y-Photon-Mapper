/**
 * @file Scale.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Mat4.hh"

/// @brief A scaling matrix.
class Scale : public Mat4 {
public:
    /**
     * @brief Construct a new Scale object
     *
     * @param sx x scaling factor
     * @param sy y scaling factor
     * @param sz z scaling factor
     */
    Scale(double sx, double sy, double sz);

    virtual Mat4 inv() const override;
};