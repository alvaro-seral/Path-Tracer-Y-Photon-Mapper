/**
 * @file Ray.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Ray.hh"

Ray::Ray(const Point& origin, const Direction& direction) :
    origin(origin),
    direction(direction.norm())
{
}