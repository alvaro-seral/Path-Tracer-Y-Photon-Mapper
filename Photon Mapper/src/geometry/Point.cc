/**
 * @file Point.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Point.hh"

Point::Point() : Coordinate(Vec3(), 1) { }
Point::Point(double x, double y, double z) : Coordinate(x, y, z, 1) { }
Point::Point(const Vec3& v) : Coordinate(v, 1) { }