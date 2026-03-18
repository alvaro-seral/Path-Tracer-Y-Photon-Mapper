/**
 * @file Mat4.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include <array>
#include <ostream>
#include <string>

/// @brief A 4x4 matrix.
class Mat4 {
protected:
    // Matrix data
    std::array<std::array<double, 4>, 4> mat = {};

public:
    /// @brief Construct a new Mat4 object
    Mat4() = default;

    /**
     * @brief Construct a new Mat 4 object
     *
     * @param m Matrix data
     */
    Mat4(const double m[4][4]);

    /**
     * @brief Construct a new Mat 4 object
     *
     * @param m Matrix data
     */
    Mat4(const std::array<std::array<double, 4>, 4>& m);

    /**
     * @brief Return a reference to the element at (i, j)
     *
     * @param i Row index
     * @param j Column index
     * @return Reference to element at (i, j)
     */
    double& operator()(int i, int j);

    /**
     * @brief Return the element at (i, j)
     *
     * @param i Row index
     * @param j Column index
     * @return Element at (i, j)
     */
    double operator()(int i, int j) const;

    /**
     * @brief Compare two matrices
     *
     * @param m Matrix
     * @return Matrices are equal
     */
    bool operator==(const Mat4& m) const;

    /**
     * @brief Add two matrices
     *
     * @param m Matrix
     * @return Sum of matrices
     */
    Mat4 operator+(const Mat4& m) const;

    /**
     * @brief Subtract two matrices
     *
     * @param m Matrix
     * @return Difference of matrices
     */
    Mat4 operator-(const Mat4& m) const;

    /**
     * @brief Multiply two matrices
     *
     * @param m Matrix
     * @return Product of matrices
     */
    Mat4 operator*(const Mat4& m) const;

    /**
     * @brief Multiply a matrix by a scalar
     *
     * @param s Scalar
     * @return Product of matrix and scalar
     */
    Mat4 operator*(double s) const;

    /**
     * @brief Multiply a scalar by a matrix
     *
     * @param s Scalar
     * @param m Matrix
     * @return Product of scalar and matrix
     */
    friend Mat4 operator*(double s, const Mat4& m);

    /**
     * @brief Divide a matrix by a scalar
     *
     * @param s Scalar
     * @return Division of matrix by scalar
     */
    Mat4 operator/(double s) const;

    /// @brief Return the transpose of the matrix
    Mat4 T() const;

    /// @brief Return the adjugate of the matrix
    Mat4 adj() const;

    /// @brief Return the determinant of the matrix
    double det() const;

    /// @brief Return the inverse of the matrix
    virtual Mat4 inv() const;

    /**
     * @brief Return the cofactor of the element at (i, j).
     *
     * @param i Row index
     * @param j Column index
     * @return Co-factor of element at (i, j)
     */
    double cof(int i, int j) const;

    /**
     * @brief Insert the matrix into an output stream.
     *
     * @param os Output stream
     * @param m Matrix
     * @return Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Mat4& m);

    /// @brief Return a string representation of the matrix.
    std::string to_string() const;
};