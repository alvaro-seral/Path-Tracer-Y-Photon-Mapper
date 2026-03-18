/**
 * @file RGB.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "RGB.hh"

RGB::RGB(double r, double g, double b) : Vec3(r, g, b) { }
RGB::RGB(const Vec3& v) : Vec3(v) { }

double& RGB::r() { return x; }
double  RGB::r() const { return x; }

double& RGB::g() { return y; }
double  RGB::g() const { return y; }

double& RGB::b() { return z; }
double  RGB::b() const { return z; }