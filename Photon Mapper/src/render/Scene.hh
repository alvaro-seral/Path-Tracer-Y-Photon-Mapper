/**
 * @file Scene.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "illumination/PointLight.hh"
#include "primitives/Primitive.hh"

#include <memory>
#include <utility>
#include <vector>

/// @brief A scene containing objects and lights
class Scene {
public:
    /// @brief List of objects in the scene
    std::vector<std::shared_ptr<Primitive>> objects = {};

    /// @brief List of lights in the scene
    std::vector<std::shared_ptr<PointLight>> lights = {};

    /// @brief Construct a new Scene object
    Scene() = default;

    /**
     * @brief Construct a new Scene object
     *
     * @param objects List of objects
     * @param lights List of lights
     */
    Scene(const std::vector<std::shared_ptr<Primitive>>&  objects,
          const std::vector<std::shared_ptr<PointLight>>& lights);

    /**
     * @brief Return the closest intersection of a ray to an object in the
     * scene. If found, returns the distance to the intersection and the object.
     * If not, returns INFINITY and nullptr.
     *
     * @param ray Ray to intersect
     * @return Pair of distance and primitive object (default INFINITY, nullptr)
     */
    std::pair<double, std::shared_ptr<Primitive>>
    closest_intersection(const Ray& ray) const;
};