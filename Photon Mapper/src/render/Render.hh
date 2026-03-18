/**
 * @file Render.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "Camera.hh"
#include "Scene.hh"
#include "image/Image.hh"

/// @brief Implementation of various rendering algorithms.
namespace Render {

/**
 * @brief Renders an image using the specified camera and scene.
 * 
 * @param camera The camera used for rendering.
 * @param scene The scene to be rendered.
 * @param paths Number of paths to trace.
 * @param num_walks Number of walks per path.
 * @param max_nearest Maximum number of nearest neighbors to consider.
 * @param max_radius Maximum radius for photon mapping.
 * @param with_next_event Whether to use next event estimation.
 * @param kernelType Type of kernel to use for rendering.
 * @param aperture The aperture size for depth of field effects (default is 0.0).
 * @param focal_distance The focal distance for depth of field effects (default is 0.0).
 * @return Rendered image as an Image object.
 */
Image render(const Camera& camera, const Scene& scene, unsigned int paths,
             unsigned int num_walks, unsigned int max_nearest, double max_radius, 
             bool with_next_event, std::string kernelType, double aperture = 0.0,
             double focal_distance = 0.0);

}