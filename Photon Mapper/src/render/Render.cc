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
 * @brief Render a pixel using photon mapping.
 *
 * @param canvas Image canvas
 * @param camera Camera
 * @param scene Scene
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @param paths Number of light paths sampled per pixel
 * @param photon_map Photon map
 * @param max_nearest Maximum number of nearest neighbours to consider
 * @param max_radius Maximum radius for neighbours
 * @param with_next_event Whether to use next event estimation
 * @param gaussian_kernel Whether to use Gaussian kernel for density estimation
 * @param aperture Aperture of the camera (for depth of field)
 * @param focal_distance Focal distance of the camera (for depth of field)
 */
void photon_mapper(std::vector<RGB>& canvas, const Camera& camera,
                    const Scene& scene, unsigned int x, unsigned int y,
                    unsigned int paths, PhotonMap& photon_map,
                    unsigned int max_nearest, double max_radius,
                    bool with_next_event, std::string kernelType, double aperture,
                    double focal_distance)
{
    for (Ray r : camera.cast_ray(x, y, paths, aperture, focal_distance)) {
        RGB L_out = {};
        L_out = L_out + read_photon_map(scene, r, photon_map, 
                                        max_nearest, max_radius,
                                        with_next_event, kernelType);
        canvas[x + y * camera.width] = canvas[x + y * camera.width] + L_out;
    }
    // Average pixel color
    canvas[x + y * camera.width] = canvas[x + y * camera.width] / paths;
}

Image render(const Camera& camera, const Scene& scene, unsigned int paths,
             unsigned int num_walks, unsigned int max_nearest, double max_radius, 
             bool with_next_event, std::string kernelType, double aperture,
             double focal_distance)
{
    std::vector<RGB> canvas(camera.width * camera.height, {0, 0, 0});
    std::list<Photon> photon_list = {};

    // Get sum of all point light sources
    double total_power = 0;
    for (auto light : scene.lights) {
        total_power += light->power.max();
    }

    for (auto light : scene.lights) {

        // Get number of walks for this light source
        unsigned int walks = (unsigned int)(num_walks * (light->power.max() / total_power));
        
        // Run each photon random walk for this light source
        for (unsigned int i = 0; i < walks; i++) {
            Ray ray = light->sample_ray();
            RGB flux = 4 * M_PI * light->power / walks;
            photon_list.splice(photon_list.end(), photon_random_walk(scene, ray, flux, with_next_event));  
        }
    }

    // Store the photons in the map
    PhotonMap photon_map = PhotonMap(photon_list, PhotonAxisPosition());

    // Render the pixels from the scene
    for (unsigned int height = 0; height < camera.height; height++) {
        for (unsigned int width = 0; width < camera.width; width++) {
            Logger::debug("Rendering pixel [" + std::to_string(width) + ", "
                          + std::to_string(height) + "]");
            
            photon_mapper(canvas, camera, scene, width, height, paths,
                          photon_map, max_nearest, max_radius, with_next_event, 
                          kernelType, aperture, focal_distance);
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