/**
 * @file Image.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Image.hh"
#include <utils/Logger.hh>

#include <algorithm>
#include <cmath>

Image::Image(unsigned int width, unsigned int height,
             const std::vector<RGB>& data, unsigned int disk_max,
             double mem_max) :
    width(width),
    height(height),
    data(data),
    disk_max(disk_max),
    mem_max(mem_max)
{
}

RGB& Image::operator()(unsigned int x, unsigned int y)
{
    return data[y * width + x];
}

const RGB& Image::operator()(unsigned int x, unsigned int y) const
{
    return data[y * width + x];
}

Image Image::clamp(double max, unsigned int _disk_max) const
{
    Logger::debug("Clamping: [0, " + std::to_string(max) + "]");

    std::vector<RGB> res(width * height);

    for (unsigned int i = 0; i < width * height; i++) {
        res[i] = {std::min(data[i].r(), max), std::min(data[i].g(), max),
                  std::min(data[i].b(), max)};
    }

    return Image(width, height, res, _disk_max, max);
}

Image Image::equalize(unsigned int _disk_max) const
{
    Logger::debug("Equalizing");

    double max_r = 0, max_g = 0, max_b = 0;

    for (auto pixel : data) {
        max_r = std::max(max_r, pixel.r());
        max_g = std::max(max_g, pixel.g());
        max_b = std::max(max_b, pixel.b());
    }

    Logger::debug("Max R: " + std::to_string(max_r));
    Logger::debug("Max G: " + std::to_string(max_g));
    Logger::debug("Max B: " + std::to_string(max_b));

    std::vector<RGB> res(width * height);

    for (unsigned int i = 0; i < width * height; i++) {
        res[i] = {
            data[i].r() / max_r,
            data[i].g() / max_g,
            data[i].b() / max_b,
        };
    }

    return Image(width, height, res, _disk_max, 1);
}

Image Image::gamma(double y, unsigned int _disk_max) const
{
    Logger::debug("Gamma correction: " + std::to_string(y));

    std::vector<RGB> res(width * height);

    for (unsigned int i = 0; i < width * height; i++) {
        res[i] = {
            mem_max * std::pow(data[i].r() / mem_max, 1.0 / y),
            mem_max * std::pow(data[i].g() / mem_max, 1.0 / y),
            mem_max * std::pow(data[i].b() / mem_max, 1.0 / y),
        };
    }

    return {width, height, res, _disk_max, mem_max};
}