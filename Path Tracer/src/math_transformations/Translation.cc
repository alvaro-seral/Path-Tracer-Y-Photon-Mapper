/**
 * @file Translation.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Translation.hh"

Translation::Translation(double tx, double ty, double tz) :
    Mat4({
        {{1, 0, 0, tx}, {0, 1, 0, ty}, {0, 0, 1, tz}, {0, 0, 0, 1}}
})
{
}

Mat4 Translation::inv() const
{
    return Translation(-mat[0][3], -mat[1][3], -mat[2][3]);
}