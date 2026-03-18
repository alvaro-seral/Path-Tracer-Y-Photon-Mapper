/**
 * @file Illumination.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "geometry/Ray.hh"
#include "primitives/Primitive.hh"
#include "render/Scene.hh"

#include <memory>

/// @brief Types of illumination events that can occur
enum Event { DIFFUSE, SPECULAR, REFRACTION, ABSORPTION, EMISSION };

/**
 * @brief Returns if a light ray is occluded by an object in the scene.
 *
 * @param scene Scene
 * @param shadow_ray Ray
 * @return True if the ray is occluded
 */
bool in_shadow(Scene scene, Ray shadow_ray);

/**
 * @brief Calculate the illumination of a intersection in the scene.
 *
 * @param scene Scene
 * @param object Object hit
 * @param intersection Point of ray hit
 * @param normal Surface normal
 * @return RGB
 */
RGB next_event_estimation(const Scene&                      scene,
                          const std::shared_ptr<Primitive>& object,
                          const Point& intersection, const Direction& normal);

/**
 * @brief Calculate global lighting by generating a random path using Monte
 * Carlo sampling.
 *
 * On each call, a next event estimation is performed to calculate the direct
 * lighting for the current bounce. If the depth is greater than 0, a new ray is
 * generated and the function is called recursively. The indirect illumination
 * is then added.
 *
 * @param scene Scene
 * @param ray Ray
 * @return Color of illumination
 */
RGB generate_random_path(const Scene& scene, const Ray& ray);
