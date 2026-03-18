/**
 * @file PPM.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#pragma once

#include "image/Image.hh"

#include <string>

/**
 * @brief Implementation of a PPM file reader and writer.
 *
 * The file follows the [PPM format
 * specification](https://netpbm.sourceforge.net/doc/ppm.html) except for the
 * following:
 *
 * - The file can have an optional #MAX=<float> line that specifies the maximum
 * color value in memory.
 * - The file can have a disk maximum above 65535.
 */
namespace PPM {

/**
 * @brief Read a PPM image from a file
 *
 * @param path Path to the file
 * @return Contents of image
 */
Image read(const std::string& path);

/**
 * @brief Write an image to a PPM file
 *
 * @param image Image
 * @param path Path to the file
 */
void write(const Image& image, const std::string& path);

/// @brief Exception thrown when reading a PPM file fails
class ReaderException : public std::exception {
public:
    virtual const char* what();
};

/// @brief Exception thrown when the magic number in a PPM file is wrong
class WrongMagicNumberException : public ReaderException {
public:
    virtual const char* what();
};

/// @brief Exception thrown when writing a PPM file fails
class WriterException : public std::exception {
public:
    virtual const char* what();
};

}