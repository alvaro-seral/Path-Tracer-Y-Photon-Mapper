/**
 * @file Mat4.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Mat4.hh"

#include <cassert>
#include <sstream>

// CONSTRUCTORES

Mat4::Mat4(const double m[4][4])
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m[i][j];
        }
    }
}

Mat4::Mat4(const std::array<std::array<double, 4>, 4>& m)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = m[i][j];
        }
    }
}

// ACCESO

double& Mat4::operator()(int i, int j) { return mat[i][j]; }

double Mat4::operator()(int i, int j) const { return mat[i][j]; }

// OPERADORES

// Con una matriz

bool Mat4::operator==(const Mat4& m) const
{
    bool res = true;
    for (int i = 0; i < 4 && res; i++) {
        for (int j = 0; j < 4 && res; j++) {
            res = mat[i][j] == m(i, j);
        }
    }
    return res;
}

Mat4 Mat4::operator+(const Mat4& m) const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = mat[i][j] + m(i, j);
        }
    }
    return res;
}

Mat4 Mat4::operator-(const Mat4& m) const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = mat[i][j] - m(i, j);
        }
    }
    return res;
}

Mat4 Mat4::operator*(const Mat4& m) const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                res(i, j) += mat[i][k] * m(k, j);
            }
        }
    }
    return res;
}

// Con un escalar

Mat4 Mat4::operator*(double s) const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = mat[i][j] * s;
        }
    }
    return res;
}

Mat4 operator*(double s, const Mat4& m) { return m * s; }

Mat4 Mat4::operator/(double s) const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = mat[i][j] / s;
        }
    }
    return res;
}

// Unarios

Mat4 Mat4::T() const
{
    Mat4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = mat[j][i];
        }
    }
    return res;
}

Mat4 Mat4::adj() const
{
    Mat4 res;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res(i, j) = cof(i, j);
        }
    }

    return res.T();
}

double Mat4::cof(int i, int j) const
{
    double m[3][3];
    int    x = 0, y = 0;

    for (int k = 0; k < 4; k++) {
        if (k == i) continue;

        for (int l = 0; l < 4; l++) {
            if (l == j) continue;

            m[x][y] = mat[k][l];
            y++;
        }

        x++;
        y = 0;
    }

    int sign = (i + j) % 2 == 0 ? 1 : -1;

    return sign
        * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
           - m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0])
           + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
}

double Mat4::det() const
{
    double res = 0;

    for (int i = 0; i < 4; i++) {
        res += mat[0][i] * cof(0, i);
    }

    return res;
}

Mat4 Mat4::inv() const
{
    assert(det() != 0);
    return adj() / det();
}

// SALIDA

std::ostream& operator<<(std::ostream& os, const Mat4& m)
{
    int max_width = 0;

    // Obtener el ancho máximo de los números
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::stringstream ss;
            ss << m.mat[i][j];
            int width = (int)ss.str().length();

            if (width > max_width) {
                max_width = width;
            }
        }
    }

    // Imprimir la matriz
    for (int i = 0; i < 4; i++) {
        os << ((i == 0) ? "/ " : (i == 3) ? "\\ " : "| ");

        for (int j = 0; j < 4; j++) {
            os.width(max_width);
            os << m.mat[i][j] << " ";
        }

        os << ((i == 0) ? "\\ " : (i == 3) ? "/ " : "| ");

        if (i != 3) os << "\n";
    }

    return os;
}

std::string Mat4::to_string() const
{
    return operator<<(std::stringstream(), *this).str();
}