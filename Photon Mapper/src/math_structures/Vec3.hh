/**
 * @file Vec3.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include <ostream>
#include <string>

/// @brief A 3D vector.
class Vec3 {
public:
    /// @brief x component
    double x = 0;

    /// @brief y component
    double y = 0;

    /// @brief z component
    double z = 0;

    /// @brief Construct a new Vec3 object
    Vec3() = default;

    /**
     * @brief Construct a new Vec 3 object
     *
     * @param x x component
     * @param y y component
     * @param z z component
     */
    Vec3(double x, double y, double z);

    /**
     * @brief Construct a new Vec 3 object
     *
     * @param v Vector
     */
    Vec3(const Vec3& v);

    /**
     * @brief Return a reference to the element of the vector at the given
     * index.
     *
     * @param i Index
     * @return Reference to the vector element.
     */
    double& operator[](std::size_t i);

    /**
     * @brief Return a constant reference to the element of the vector at the
     * given index.
     *
     * @param i Index
     * @return Constant reference to the vector element.
     */
    const double& operator[](std::size_t i) const;

    /**
     * @brief Compare two vectors.
     *
     * @param v Vector
     * @return The vectors are equal.
     */
    bool operator==(const Vec3& v) const;

    /**
     * @brief Assign the values of another vector to the vector.
     *
     * @param v Vector
     * @returns Reference to the vector.
     */
    Vec3& operator=(const Vec3& v);

    /**
     * @brief Add a vector to the vector.
     *
     * @param v Vector
     * @return Reference to the vector.
     */
    Vec3& operator+=(const Vec3& v);

    /**
     * @brief Subtract a vector from the vector.
     *
     * @param v  Vector
     * @return Reference to the vector.
     */
    Vec3& operator-=(const Vec3& v);

    /**
     * @brief Multiply the vector by a scalar.
     *
     * @param s Scalar
     * @return Reference to the vector.
     */
    Vec3& operator*=(double s);

    /**
     * @brief Divide the vector by a scalar.
     *
     * @param s  Scalar
     * @return Reference to the vector.
     */
    Vec3& operator/=(double s);

    /**
     * @brief Add two vectors.
     *
     * @param v Vector
     * @return Sum of vectors.
     */
    Vec3 operator+(const Vec3& v) const;

    /**
     * @brief Subtract two vectors.
     *
     * @param v Vector
     * @return Difference of vectors.
     */
    Vec3 operator-(const Vec3& v) const;

    /**
     * @brief Dot product of two vectors.
     *
     * @param v Vector
     * @return Dot product of vectors.
     */
    double operator*(const Vec3& v) const;

    /**
     * @brief Cross product of two vectors.
     *
     * @param v1 Left side vector
     * @param v2 Right side vector
     * @return Cross product of vectors.
     */
    friend Vec3 cross(const Vec3& v1, const Vec3& v2);

    /**
     * @brief Element-wise multiplication of two vectors.
     *
     * @param v1 Left side vector
     * @param v2 Right side vector
     * @return Element-wise product of vectors.
     */
    friend Vec3 times(const Vec3& v1, const Vec3& v2);

    /**
     * @brief Multiply the vector by a scalar.
     *
     * @param s Scalar
     * @return Product of vector and scalar.
     */
    Vec3 operator*(double s) const;

    /**
     * @brief Multiply a scalar by a vector.
     *
     * @param s Scalar
     * @param v Vector
     * @return Product of scalar and vector.
     */
    friend Vec3 operator*(double s, const Vec3& v);

    /**
     * @brief Divide the vector by a scalar.
     *
     * @param s Scalar
     * @return Quotient of vector and scalar.
     */
    Vec3 operator/(double s) const;

    /// @brief Return the negated of the vector.
    Vec3 operator-() const;

    /// @brief Return the modulus of the vector.
    double mod() const;

    /// @brief Return the normalized vector.
    Vec3 norm() const;

    /// @brief Return the max value between x, y and z.
    double max() const;

    /** @brief Insert the vector into an output stream.
     *
     * @param os Output stream
     * @param v Vector
     * @return Output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

    /// @brief Return the string representation of the vector.
    std::string to_string() const;
};