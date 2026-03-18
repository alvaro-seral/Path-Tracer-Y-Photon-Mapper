/**
 * @file Image.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "RGB.hh"

#include <vector>

/// @brief Stores an image and its properties in memory
class Image {
public:
    /// @brief Width of image
    const unsigned int width = 0;

    /// @brief Size of image
    const unsigned int height = 0;

    /// @brief Canvas
    std::vector<RGB> data = {};

    /// @brief Maximum color value on disk
    const unsigned int disk_max = 255;

    /// @brief Maximum color value in memory
    const double mem_max = 1;

    /// @brief Construct a new Image object
    Image() = default;

    /**
     * @brief Construct a new Image object
     *
     * @param width Width of the image
     * @param height Height of the image
     * @param data Image data
     * @param disk_max Maximum color value on disk
     * @param mem_max Maximum color value in memory
     */
    Image(unsigned int width, unsigned int height, const std::vector<RGB>& data,
          unsigned int disk_max = 255, double mem_max = 1);

    /**
     * @brief Return a reference for the pixel at (x, y)
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return Reference to RGB tuple
     */
    RGB& operator()(unsigned int x, unsigned int y);

    /**
     * @brief Return the value for the pixel at (x, y)
     *
     * @param x x coordinate
     * @param y y coordinate
     * @return Value of RGB tuple
     */
    const RGB& operator()(unsigned int x, unsigned int y) const;

    /**
     * @brief Apply the clamping tone operator on the image.
     *
     * \f$out(x,y) = out(x,y) \ if \ out(x,y) < max \ else \ max\f$
     *
     * @param max Maximum value in memory of the resulting image
     * @param _disk_max New disk maximum value
     * @return Image
     */
    Image clamp(double max = 1.0, unsigned int _disk_max = 255) const;

    /**
     * @brief Apply the equalization tone operator on the image, normalizing the
     * data to a [0, 1] range.
     *
     * \f$out(x,y) = out(x,y) / max(out)\f$
     *
     * @param _disk_max New disk maximum value
     * @return Image
     */
    Image equalize(unsigned int _disk_max = 255) const;

    /**
     * @brief Apply the gamma correction tone operator on the image.
     *
     * \f$out(x,y) = out(x,y)^{1/\gamma}\f$
     *
     * @param y Gamma factor
     * @param _disk_max New disk maximum value
     * @return Image
     */
    Image gamma(double y = 2.2, unsigned int _disk_max = 255) const;
};