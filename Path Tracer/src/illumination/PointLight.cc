/**
 * @file PointLight.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "PointLight.hh"

PointLight::PointLight(const Point& center, const RGB& power) :
    center(center),
    power(power)
{
}