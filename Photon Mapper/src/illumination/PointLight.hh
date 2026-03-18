/**
 * @file PointLight.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "geometry/Point.hh"
#include "geometry/Ray.hh"
#include "image/RGB.hh"

/// @brief A point light source
class PointLight {
public:
    /// @brief Position of the light source
    Point center;

    /// @brief Power of the light source
    RGB power;

    /**
     * @brief Construct a new Point Light object
     *
     * @param center Position of the light source
     * @param power Power of the light source
     */
    PointLight(const Point& center, const RGB& power);

    /** @brief Sample a ray using uniform angle sampling */
    Ray sample_ray() const;
};