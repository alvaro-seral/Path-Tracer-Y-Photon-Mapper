/**
 * @file Rotation.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Rotation.hh"

Rotation::Rotation(double theta_x, double theta_y, double theta_z) :
    x(theta_x),
    y(theta_y),
    z(theta_z)
{
    Mat4 m = z * y * x;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m(i, j);
        }
    }
}

Mat4 Rotation::inv() const { return x.inv() * y.inv() * z.inv(); }