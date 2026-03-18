/**
 * @file RotX.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "RotX.hh"

#include <cmath>

RotX::RotX(double theta) :
    Mat4({
        {{1, 0, 0, 0},
         {0, std::cos(theta), -std::sin(theta), 0},
         {0, std::sin(theta), std::cos(theta), 0},
         {0, 0, 0, 1}}
})
{
}

Mat4 RotX::inv() const { return this->T(); }