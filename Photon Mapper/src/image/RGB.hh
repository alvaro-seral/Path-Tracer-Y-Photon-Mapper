/**
 * @file RGB.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "math_structures/Vec3.hh"

/// @brief A red-green-blue tuple
class RGB : public Vec3 {
public:
    /// @brief Construct a new RGB object
    RGB() = default;

    /**
     * @brief Construct a new RGB object
     *
     * @param r Red component
     * @param g Green component
     * @param b Blue component
     */
    RGB(double r, double g, double b);

    /**
     * @brief Construct a new RGB object
     *
     * @param v Vector with the RGB components
     */
    RGB(const Vec3& v);

    /// @brief Return a reference of the red component
    double& r();

    /// @brief Return the red component
    double r() const;

    /// @brief Return a reference of the green component
    double& g();

    /// @brief Return the green component
    double g() const;

    /// @brief Return a reference of the blue component
    double& b();

    /// @brief Return the blue component
    double b() const;
};