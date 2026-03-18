/**
 * @file RotY.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "RotY.hh"

#include <cmath>

RotY::RotY(double theta) :
    Mat4({
        {{std::cos(theta), 0, std::sin(theta), 0},
         {0, 1, 0, 0},
         {-std::sin(theta), 0, std::cos(theta), 0},
         {0, 0, 0, 1}}
})
{
}

Mat4 RotY::inv() const { return this->T(); }