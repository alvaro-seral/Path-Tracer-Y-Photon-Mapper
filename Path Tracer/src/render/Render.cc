/**
 * @file Render.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Render.hh"
#include "illumination/Illumination.hh"
#include "primitives/Plane.hh"
#include "utils/Logger.hh"
#include "utils/RNG.hh"

#include <cassert>
#include <cmath>
#include <iostream>
#include <memory>

namespace Render {

/**
 * @brief Render a pixel using ray tracing. The color is determined only by the
 * object the rays intersect.
 *
 * @param canvas Image canvas
 * @param camera Camera
 * @param scene Scene
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param paths Number of samples per pixel
 */
void ray_tracer(std::vector<RGB>& canvas, const Camera& camera,
                const Scene& scene, unsigned int x, unsigned int y,
                unsigned int paths)
{
    for (Ray r : camera.cast_ray(x, y, paths)) {
        auto   closest_intersection = scene.closest_intersection(r);
        double distance = closest_intersection.first;
        std::shared_ptr<Primitive> closest_object = closest_intersection.second;

        if (!(distance < INFINITY && distance > 0)) continue;

        // Set pixel color
        canvas[x + y * camera.width]
            = canvas[x + y * camera.width] + closest_object->material.kd;
    }

    // Average pixel color
    canvas[x + y * camera.width] = canvas[x + y * camera.width] / paths;
}

/**
 * @brief Render a pixel using direct lighting path tracing. The color is
 * determined by the next event estimation of the object the rays intersect.
 *
 * @param canvas Image canvas
 * @param camera Camera
 * @param scene Scene
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param paths Number of samples per pixel
 */
void path_tracer_direct_lighting(std::vector<RGB>& canvas, const Camera& camera,
                                 const Scene& scene, unsigned int x,
                                 unsigned int y, unsigned int paths)
{

    for (Ray r : camera.cast_ray(x, y, paths)) {
        auto   closest_intersection = scene.closest_intersection(r);
        double distance = closest_intersection.first;
        std::shared_ptr<Primitive> closest_object = closest_intersection.second;

        if (!(distance < INFINITY && distance > 0)) continue;

        Point     intersection = r.origin + r.direction * distance;
        Direction normal = closest_object->normalTo(intersection);

        canvas[x + y * camera.width] = canvas[x + y * camera.width]
            + next_event_estimation(scene, closest_object, intersection,
                                    normal);
    }

    canvas[x + y * camera.width] = canvas[x + y * camera.width] / paths;
}

/**
 * @brief Render a pixel using path tracing. The color is determined by the
 * global illumination of the object the rays intersect.
 *
 * @param canvas Image canvas
 * @param camera Camera
 * @param scene Scene
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param paths Number of light paths sampled per pixel
 * @param aperture Aperture of the camera (for depth of field)
 * @param focal_distancel Focal distance of the camera (for depth of field)
 */
void path_tracer(std::vector<RGB>& canvas, const Camera& camera,
                 const Scene& scene, unsigned int x, unsigned int y,
                 unsigned int paths, double aperture,
                 double focal_distancel)
{
    for (Ray r : camera.cast_ray(x, y, paths, aperture, focal_distancel)) {
        RGB L_out = {};
        L_out = L_out + generate_random_path(scene, r);
        canvas[x + y * camera.width] = canvas[x + y * camera.width] + L_out;
    }
    // Average pixel color
    canvas[x + y * camera.width] = canvas[x + y * camera.width] / paths;
}

Image render(const Camera& camera, const Scene& scene, const RenderMode mode,
             unsigned int paths, double aperture, 
             double focal_distancel)
{
    std::vector<RGB> canvas(camera.width * camera.height, {0, 0, 0});

    for (unsigned int height = 0; height < camera.height; height++) {
        for (unsigned int width = 0; width < camera.width; width++) {
            Logger::debug("Rendering pixel [" + std::to_string(width) + ", "
                          + std::to_string(height) + "]");

            if (mode == RAY_TRACER)
                ray_tracer(canvas, camera, scene, width, height, paths);
            else if (mode == PATH_TRACER_DIRECT_LIGHTING)
                path_tracer_direct_lighting(canvas, camera, scene, width,
                                            height, paths);
            else if (mode == PATH_TRACER)
                path_tracer(canvas, camera, scene, width, height, paths,
                            aperture, focal_distancel);
        }
    }

    // Process and return image
    double       mem_max = 1;
    unsigned int disk_max = 255;

    // Find max value in canvas (minimum is 1)
    for (const RGB& color : canvas) {
        mem_max = std::max(mem_max,
                           std::max(color.r(), std::max(color.g(), color.b())));
    }

    return Image(camera.width, camera.height, canvas, disk_max, mem_max);
}
}