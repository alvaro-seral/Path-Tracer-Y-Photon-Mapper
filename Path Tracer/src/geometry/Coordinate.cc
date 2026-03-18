/**
 * @file Coordinate.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Coordinate.hh"

Coordinate::Coordinate(double x, double y, double z, double w) :
    Vec3(x, y, z),
    w(w)
{
}

Coordinate::Coordinate(const Vec3& v, double w) : Vec3(v), w(w) { }

Coordinate::Coordinate(const Coordinate& c) : Vec3(c), w(c.w) { }

Coordinate Coordinate::operator=(const Coordinate& c)
{
    if (this != &c) {
        x = c.x;
        y = c.y;
        z = c.z;
        w = c.w;
    }

    return *this;
}

Coordinate operator*(const Mat4& m, const Coordinate& c)
{
    return Coordinate(
        m(0, 0) * c.x + m(0, 1) * c.y + m(0, 2) * c.z + m(0, 3) * c.w,
        m(1, 0) * c.x + m(1, 1) * c.y + m(1, 2) * c.z + m(1, 3) * c.w,
        m(2, 0) * c.x + m(2, 1) * c.y + m(2, 2) * c.z + m(2, 3) * c.w,
        m(3, 0) * c.x + m(3, 1) * c.y + m(3, 2) * c.z + m(3, 3) * c.w);
}