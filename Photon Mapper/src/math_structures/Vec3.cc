/**
 * @file Vec3.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Vec3.hh"

#include <cassert>
#include <cmath>
#include <sstream>

// CONSTRUCTORES

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z) { }

Vec3::Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) { }

// OPERADORES

// Acceso

double& Vec3::operator[](std::size_t i)
{
    assert(i >= 0 && i <= 2);
    return (i == 0) ? x : (i == 1) ? y : z;
}

const double& Vec3::operator[](std::size_t i) const
{
    assert(i >= 0 && i <= 2);
    return (i == 0) ? x : (i == 1) ? y : z;
}

// Con un vector

bool Vec3::operator==(const Vec3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

Vec3& Vec3::operator=(const Vec3& v)
{
    if (this != &v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }
    return *this;
}

Vec3& Vec3::operator+=(const Vec3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vec3& Vec3::operator*=(double s)
{
    x *= s;
    y *= s;
    z *= s;
    return *this;
}

Vec3& Vec3::operator/=(double s)
{
    x /= s;
    y /= s;
    z /= s;
    return *this;
}

Vec3 Vec3::operator+(const Vec3& v) const
{
    return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3 Vec3::operator-(const Vec3& v) const
{
    return Vec3(x - v.x, y - v.y, z - v.z);
}

double Vec3::operator*(const Vec3& v) const
{
    return x * v.x + y * v.y + z * v.z;
}

Vec3 cross(const Vec3& a, const Vec3& b)
{
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

Vec3 times(const Vec3& v1, const Vec3& v2)
{
    return {v1.x * v2.x, v1.y * v2.y, v1.z * v2.z};
}

// Con un escalar

Vec3 Vec3::operator*(double s) const { return {x * s, y * s, z * s}; }

Vec3 operator*(double s, const Vec3& v) { return v * s; }

Vec3 Vec3::operator/(double s) const { return {x / s, y / s, z / s}; }

// Unarios

Vec3 Vec3::operator-() const { return {-x, -y, -z}; }

double Vec3::mod() const { return std::sqrt(x * x + y * y + z * z); }

Vec3 Vec3::norm() const { return *this / mod(); }

double Vec3::max() const { return std::max(std::max(x, y), z); }

// SALIDA

std::ostream& operator<<(std::ostream& os, const Vec3& v)
{
    return (os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")");
}

std::string Vec3::to_string() const
{
    return operator<<(std::stringstream(), *this).str();
}
