/**
 * @file Illumination.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "geometry/Ray.hh"
#include "photon/Photon.hh"
#include "primitives/Primitive.hh"
#include "render/Scene.hh"

#include <memory>
#include <vector>
#include <list>

/// @brief Types of illumination events that can occur
enum Event { DIFFUSE, SPECULAR, REFRACTION, ABSORPTION, EMISSION };

/// @brief Types of kernel that can be used for photon density estimation
enum KernelType { UNIFORM, GAUSSIAN, TRIANGULAR, EPANECHNIKOV, COSINE };

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
 * @param outgoing_ray Ray
 * @return RGB
 */
RGB next_event_estimation(const Scene&                      scene,
                          const std::shared_ptr<Primitive>& object,
                          const Point& intersection, const Direction& normal);

/**
 * @brief Perform a random walk of photons in the scene.
 *
 * @param scene Scene to check
 * @param ray Ray to check
 * @param flux Flux of the photon
 * @param with_next_event If true, the next event will be estimated
 * @return std::list<Photon> List of photons
 */
std::list<Photon> photon_random_walk(const Scene& scene, const Ray& ray, 
                                     const RGB& flux, 
                                     bool with_next_event);

/** @brief Read the photon map and return the color at the intersection point.
 *
 * @param scene Scene to check
 * @param ray Ray to check
 * @param photon_map Photon map to use
 * @param max_nearest Maximum number of nearest neighbors to consider
 * @param max_radius Maximum radius to consider for the estimation
 * @param with_next_event If true, the next event will be estimated
 * @param kernelType Type of kernel to use for the estimation
 * @return RGB Estimated color at the intersection point
 */
RGB read_photon_map(const Scene& scene, const Ray& ray, PhotonMap& photon_map,
                    unsigned int max_nearest, double max_radius, 
                    bool with_next_event, std::string kernelType);
