/**
 * @file Primitive.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include <memory>

#include "geometry/Direction.hh"
#include "geometry/Point.hh"
#include "geometry/Ray.hh"
#include "image/RGB.hh"
#include "materials/Material.hh"
#include "math_transformations/ChangeOfBasis.hh"

/// @brief A primitive geometric object.
class Primitive {

protected:
    /// @brief Error margin
    const double EPSILON = 1e-6;

    /**
     * @brief Construct a new Primitive object
     *
     * @param material
     */
    Primitive(const Material& material);

    virtual ~Primitive();

public:
    const Material material;

    /**
     * @brief Returns the distance to the intersection point with the ray. If
     * the ray does not intersect, return INFINITY.
     *
     * @param ray Ray to intersect with.
     * @return Distance to the intersection point.
     */
    virtual double intersects(const Ray& ray) const = 0;

    /**
     * @brief Returns the normal to the primitive at the given point.
     *
     * @param point Surface point
     * @return Direction of normal
     */
    virtual Direction normalTo(const Point& point) const = 0;
};