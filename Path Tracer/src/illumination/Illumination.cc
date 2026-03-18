/**
 * @file Illumination.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Illumination.hh"
#include "utils/Logger.hh"
#include "utils/RNG.hh"

#include <algorithm>
#include <cmath>

/**
 * @brief Determine the event of the bounce using a russian roulette
 * @param object Primitive object
 * @return Event of the bounce
 */
Event russian_roulette(const std::shared_ptr<Primitive>& object)
{
    if (object->material.pe > 0) return EMISSION;

    double n = uniform(0, 1);
    if (n < object->material.pd) return DIFFUSE;
    if (n < object->material.pd + object->material.ps) return SPECULAR;
    if (n < object->material.pd + object->material.ps + object->material.pt)
        return REFRACTION;

    return ABSORPTION;
}

/**
 * @brief Sample a new ray for a diffuse bounce using cosine sampling.
 * @param intersection Point of intersection
 * @param normal Normal to the surface
 * @return Ray
 */
Ray sample_diffuse_ray(const Point& intersection, const Direction& normal)
{
    // Get new basis
    Point     o = intersection;
    Direction n = normal;
    Direction aux = (std::abs((n * Direction {1, 0, 0})) > 0.9999)
        ? Direction {0, 1, 0}
        : Direction {1, 0, 0};
    Direction i = cross(n, aux); // i es perpendicular a n
    Direction j = cross(n, i);   // j es perpendicular a n y a i

    ChangeOfBasis T = {o, i, j, n};

    // Generate sample
    double c_theta = std::acos(std::sqrt(1 - uniform(0, 1)));
    double c_phi = 2 * M_PI * uniform(0, 1);

    // Get direction of bounce
    Direction w_in_p = {std::sin(c_theta) * std::cos(c_phi),
                        std::sin(c_theta) * std::sin(c_phi), std::cos(c_theta)};

    Direction w_in = T * w_in_p;

    // Get next intersection
    Ray new_ray = {intersection + w_in * 0.0001, w_in};
    return new_ray;
}

/**
 * @brief Return the resulting ray of a specular bounce
 *
 * @param r Incoming ray
 * @param intersection Point of intersection
 * @param normal Normal to the surface
 * @return Ray
 */
Ray specular_ray(const Ray& r, const Point& intersection,
                 const Direction& normal)
{
    // Get direction of bounce
    Direction w_in = r.direction - 2.0 * (r.direction * normal) * normal;

    // Get next intersection
    Ray new_ray = {intersection + w_in * 0.0001, w_in};
    return new_ray;
}

/**
 * @brief Return the resulting ray of a refraction bounce
 *
 * @param r Incoming ray
 * @param intersection Intersection point
 * @param normal Normal to the surface
 * @param nt Refraction index
 * @return Ray
 */
Ray refraction_ray(const Ray& r, const Point& intersection,
                   const Direction& normal, const double nt)
{
    // Get direction of bounce
    double    c_theta = std::clamp(r.direction * normal, -1.0, 1.0);
    double    eta_1 = 1, eta_2 = nt;
    Direction normal_aux = normal;

    if (c_theta < 0) {
        c_theta = -c_theta;
    } else {
        double eta_aux = eta_1;
        eta_1 = eta_2;
        eta_2 = eta_aux;
        normal_aux = -normal;
    }

    double eta = eta_1 / eta_2;
    double k = 1 - eta * eta * (1 - c_theta * c_theta);

    Direction w_in = {0, 0, 0};
    if (k >= 0) {
        w_in = eta * r.direction + (eta * c_theta - sqrt(k)) * normal_aux;
    }

    // Get next intersection
    Ray new_ray = {intersection + w_in * 0.0001, w_in};
    return new_ray;
}

/**
 * @brief Calculate the diffuse bidirectional reflectance distribution
 * function of the object.
 *
 * @param object Object
 * @return RGB
 */
RGB diffuse_BRDF(const std::shared_ptr<Primitive>& object)
{
    return object->material.pd > 0
        ? object->material.kd / M_PI / object->material.pd
        : RGB(0, 0, 0);
}

/**
 * @brief Calculate the specular bidirectional reflectance distribution
 * function of the object.
 *
 * @param object Object
 * @return RGB
 */
RGB specular_BRDF(const std::shared_ptr<Primitive>& object)
{
    return object->material.ps > 0
           ? object->material.ks / object->material.ps // / (new_ray * normal)
           : RGB(0, 0, 0);
}

/**
 * @brief Calculate the refraction bidirectional transmission distribution
 * function of the object.
 *
 * @param object Object
 * @return RGB
 */
RGB refraction_BTDF(const std::shared_ptr<Primitive>& object)
{
    return object->material.pt > 0
           ? object->material.kt / object->material.pt
           : RGB(0, 0, 0);
}

/**
 * @brief Calculate the bidirectional scattering distribution function of the
 * object.
 *
 * @param event Event of the bounce
 * @param object Object
 * @return RGB
 */
RGB BSDF(const Event& event, const std::shared_ptr<Primitive>& object)
{
    // If object is an area light, return emission
    if (object->material.pe > 0) return object->material.ke;

    switch (event) {
        case DIFFUSE:
            return diffuse_BRDF(object);
            break;

    case SPECULAR:
        return diffuse_BRDF(object) + specular_BRDF(object);
        break;

    case REFRACTION:
        return diffuse_BRDF(object) + refraction_BTDF(object);
        break;
    
    default:
        return RGB(0, 0, 0); // Absorption
        break;
    }
}

bool in_shadow(Scene scene, Ray shadow_ray)
{
    for (std::shared_ptr<Primitive> object : scene.objects) {
        double distance = object->intersects(shadow_ray);

        if (distance < 1 && distance >= 0) {
            return true;
        }
    }

    return false;
}

RGB next_event_estimation(const Scene&                      scene,
                          const std::shared_ptr<Primitive>& object,
                          const Point& intersection, const Direction& normal)
{
    RGB L_out = {};

    for (std::shared_ptr<PointLight> light : scene.lights) {
        Direction w_in = (light->center - intersection);
        Ray       incoming_ray = {intersection + w_in * 0.001, w_in};

        if (in_shadow(scene, incoming_ray)) continue;

        // Calculate illumination
        RGB    L_in = light->power / (w_in.mod() * w_in.mod());
        RGB    fr = object->material.kd * M_1_PI;
        double geometry = std::abs(normal * w_in.norm());

        // Set pixel color
        L_out += times(L_in, fr) * geometry;
    }

    return L_out;
}

RGB generate_random_path(const Scene& scene, const Ray& ray)
{
    // Find intersection
    auto   closest_intersection = scene.closest_intersection(ray);
    double distance = closest_intersection.first;
    std::shared_ptr<Primitive> closest_object = closest_intersection.second;

    if (!(distance < INFINITY && distance > 0)) return RGB(0, 0, 0);

    Point     intersection = (ray.origin + ray.direction * distance);
    Direction normal = closest_object->normalTo(intersection);

    // Get event from russian roulette
    Event event = russian_roulette(closest_object);

    // Absorbtion and emission events are trivial
    if (event == ABSORPTION) return RGB(0, 0, 0);
    else if (event == EMISSION) return closest_object->material.ke;
    // Diffuse, specular and refraction events require further calculations
    else {
        RGB L_out = {};

        // Get new ray
        Ray new_ray = (event == DIFFUSE)
            ? sample_diffuse_ray(intersection, normal)
            : (event == SPECULAR) ? specular_ray(ray, intersection, normal)
                                  : refraction_ray(ray, intersection, normal,
                                                   closest_object->material.nt);

        // If event is diffuse, calculate NEE
        if (event == DIFFUSE)
            L_out += next_event_estimation(scene, closest_object, intersection,
                                           normal);

        // Calculate indirect lighting
        L_out
            += times(generate_random_path(scene, new_ray),
                     BSDF(event, closest_object));

        // If diffuse, multply by PI
        if (event == DIFFUSE) L_out *= M_PI;

        return L_out;
    }

    // Fallback
    Logger::warning("No event found. Returning black.");
    return RGB(0, 0, 0);
}
