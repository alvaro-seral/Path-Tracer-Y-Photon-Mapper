/**
 * @file Camera.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "geometry/Direction.hh"
#include "geometry/Point.hh"
#include "geometry/Ray.hh"
#include "math_transformations/ChangeOfBasis.hh"

#include <vector>

/// @brief A camera.
class Camera {
private:
    /**
     * @brief Cast a number of rays from the camera to the area of a given
     * pixel, without depth of field.
     *
     * @param x Width
     * @param y Height
     * @param samples Number of samples to take
     * @return Vector of rays   
     */
    std::vector<Ray> cast_ray_simple(unsigned int x, unsigned int y,
                                     unsigned int samples) const;

    /**
     * @brief Cast a number of rays from the camera to the area of a given
     * pixel, taking into account depth of field.
     *
     * @param x Width
     * @param y Height
     * @param samples Number of samples to take
     * @param aperture Aperture of the camera
     * @param focal_distance Focal distance of the camera
     * @return Vector of rays
     */
    std::vector<Ray> cast_ray_with_depth(unsigned int x, unsigned int y,
                                         unsigned int samples, 
                                         double aperture, 
                                         double focal_distance) const;

public:
    /// @brief Width of camera view
    const unsigned int width;

    /// @brief Height of camera view
    const unsigned int height;

    /// @brief Aspect ratio of camera
    const double aspect_ratio;

    /// @brief Field of view
    const double fov;

    /// @brief Origin point of camera
    const Point origin;

    /// @brief View matrix
    const Mat4 view;

    /**
     * @brief Construct a new Camera object
     *
     * @param origin Origin point
     * @param left Left direction
     * @param up Up direction
     * @param forward Forward direction
     * @param size Size of camera view
     */
    Camera(const Point& origin, const Direction& left, const Direction& up,
           const Direction& forward, unsigned int size);

    /**
     * @brief Construct a new Camera object
     *
     * @param origin Origin point
     * @param target Target point
     * @param width Width
     * @param height Height
     * @param fov Field of view
     */
    Camera(const Point& origin, const Point& target, unsigned int width,
           unsigned int height, double fov);

    /**
     * @brief Cast a number of rays from the camera to the area of a given
     * pixel, taking into account depth of field.
     * 
     * @param x Width
     * @param y Height
     * @param samples Number of samples to take. If 1, only one ray is cast at
     * the center of the screen. Otherwise, it casts rays at random uniform
     * points of the pixel.
     * @param aperture Aperture of the camera.
     * @param focal_distance Focal distance of the camera.
     * @return Vector of rays
     */
    std::vector<Ray> cast_ray(unsigned int x, unsigned int y,
                              unsigned int samples, double aperture = 0.0, 
                              double focal_distance = 0.0) const;
};