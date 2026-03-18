/**
 * @file PointLight.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "PointLight.hh"
#include "utils/RNG.hh"

#include <cmath>

PointLight::PointLight(const Point& center, const RGB& power) :
    center(center),
    power(power)
{
}

Ray PointLight::sample_ray() const
{
    double    c_theta = std::acos(2 * uniform(0, 1) - 1);
    double    c_phi = 2 * M_PI * uniform(0, 1);
    Direction dir = {std::sin(c_theta) * std::cos(c_phi),
                     std::sin(c_theta) * std::sin(c_phi), std::cos(c_theta)};

    return Ray(center, dir);
}