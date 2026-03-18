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

/// @brief Available rendering modes
enum RenderMode { RAY_TRACER, PATH_TRACER_DIRECT_LIGHTING, PATH_TRACER };

/**
 * @brief Renders an image using the specified camera and scene.
 *
 * @param camera The camera used for rendering.
 * @param scene The scene to be rendered.
 * @param mode The rendering mode to use.
 * @param paths The number of paths to trace for path tracing modes.
 * @param aperture The aperture size for depth of field effects (default is 0.0).
 * @param focal_distance The focal distance for depth of field effects (default is 0.0).
 * @return Rendered image as an Image object.
 */
Image render(const Camera& camera, const Scene& scene, const RenderMode mode,
             unsigned int paths, double aperture = 0.0, 
             double focal_distance = 0.0);

}