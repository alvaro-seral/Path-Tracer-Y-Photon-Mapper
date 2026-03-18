/**
 * @file RotZ.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "RotZ.hh"

#include <cmath>

RotZ::RotZ(double theta) :
    Mat4({
        {{std::cos(theta), -std::sin(theta), 0, 0},
         {std::sin(theta), std::cos(theta), 0, 0},
         {0, 0, 1, 0},
         {0, 0, 0, 1}}
})
{
}

Mat4 RotZ::inv() const { return this->T(); }