/**
 * @file ChangeOfBasis.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "ChangeOfBasis.hh"

ChangeOfBasis::ChangeOfBasis(Point origin, Direction x, Direction y,
                             Direction z) :
    Mat4({
        {{x.x, y.x, z.x, origin.x},
         {x.y, y.y, z.y, origin.y},
         {x.z, y.z, z.z, origin.z},
         {0, 0, 0, 1}}
})
{
}