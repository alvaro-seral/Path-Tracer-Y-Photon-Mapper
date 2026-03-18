/**
 * @file Camera.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Camera.hh"
#include "math_transformations/ChangeOfBasis.hh"
#include "utils/RNG.hh"

#include <cassert>
#include <cmath>
#include <iostream>

Camera::Camera(const Point& origin, const Direction& left, const Direction& up,
               const Direction& forward, unsigned int size) :
    width((unsigned int)(size * (left.mod() / up.mod()))),
    height(size),
    aspect_ratio(left.mod() / up.mod()),
    fov(2 * std::atan(up.mod() / forward.mod())),
    origin(origin),
    view(ChangeOfBasis(origin, -left, up, forward))
{
}

Camera::Camera(const Point& origin, const Point& target, unsigned int width,
               unsigned int height, double fov) :
    width(width),
    height(height),
    aspect_ratio((double)width / height),
    fov(fov * M_PI / 180),
    origin(origin),
    view(ChangeOfBasis(
        origin,
        cross({0, 1, 0}, (target - origin).norm()) * std::tan(this->fov / 2)
            * (target - origin).mod() * aspect_ratio,
        Direction(0, 1, 0) * std::tan(this->fov / 2) * (target - origin).mod(),
        target - origin))
{
}

std::vector<Ray> Camera::cast_ray(unsigned int x, unsigned int y,
                                  unsigned int samples, double aperture, 
                                  double focal_distance) const
{
    if (aperture == 0.0)
        return cast_ray_simple(x, y, samples);
    else
        return cast_ray_with_depth(x, y, samples, aperture, focal_distance);
}

std::vector<Ray> Camera::cast_ray_simple(unsigned int x, unsigned int y,
                                         unsigned int samples) const
{
    assert(samples > 0);

    if (samples == 1) {
        // Find point in camera coordinates
        double u = (2 * ((double)x + 0.5) / width - 1);
        double v = (1 - 2 * ((double)y + 0.5) / height);

        // Get world coordinate
        return {Ray(origin, (view * Direction(u, v, 1)))};
    }

    std::vector<Ray> rays;

    for (unsigned int i = 0; i < samples; i++) {
        double u = (2 * ((double)x + uniform(0, 1)) / width - 1);
        double v = (1 - 2 * ((double)y + uniform(0, 1)) / height);

        rays.push_back(Ray(origin, (view * Direction(u, v, 1))));
    }

    return rays;
}

/*
    A partir de función anterior y diapositivas de clase
*/
std::vector<Ray> Camera::cast_ray_with_depth(unsigned int x, unsigned int y,
                                             unsigned int samples, 
                                             double aperture, 
                                             double focal_distance) const
{
    assert(samples > 0);

    Direction x_axis = view * Direction(1, 0, 0);
    Direction y_axis = view * Direction(0, 1, 0);

    double u_centro = (2 * ((double)x + 0.5) / width - 1);
    double v_centro = (1 - 2 * ((double)y + 0.5) / height);

    Point centro = origin 
                 + (view * Direction(u_centro, v_centro, 1)).norm() 
                 * focal_distance;

    std::vector<Ray> rays;
    for (unsigned int i = 0; i < samples; i++) {

        // double r = aperture * uniform(0, 1);
        double r = aperture * std::sqrt(uniform(0, 1)) / 2.0; // mas uniforme
        double theta = 2.0 * M_PI * uniform(0, 1);
        double u_aperture = r * std::cos(theta);
        double v_aperture = r * std::sin(theta);

        Point origin_aperture = origin 
                              + u_aperture * x_axis / (2.0 * x_axis.mod()) 
                              + v_aperture * y_axis / (2.0 * y_axis.mod());

        Direction dir = (centro - origin_aperture).norm();

        rays.push_back(Ray(origin_aperture, dir));
    }

    return rays;
}
