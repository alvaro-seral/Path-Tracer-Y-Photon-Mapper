/**
 * @file Scale.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Scale.hh"

Scale::Scale(double sx, double sy, double sz) :
    Mat4({
        {{sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}}
})
{
}

Mat4 Scale::inv() const
{
    return Scale(1 / mat[0][0], 1 / mat[1][1], 1 / mat[2][2]);
}