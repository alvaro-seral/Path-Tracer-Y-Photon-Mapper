/**
 * @file Direction.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Direction.hh"

Direction::Direction(double x, double y, double z) : Coordinate(x, y, z, 0) { }
Direction::Direction(const Vec3& v) : Coordinate(v, 0) { }