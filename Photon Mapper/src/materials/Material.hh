#pragma once

#include "image/RGB.hh"

/// @brief A type of material used by a geometric object
class Material {
public:
    /// @brief Diffuse coefficient
    const RGB kd = {0, 0, 0};

    /// @brief Diffuse probability
    const double pd = 0;

    /// @brief Specular coefficient
    const RGB ks = {0, 0, 0};

    /// @brief Specular probability
    const double ps = 0;

    /// @brief Refraction coefficient
    const RGB kt = {0, 0, 0};

    /// @brief Refraction probability
    const double pt = 0;

    /// @brief Refraction index of the primitive
    const double nt = 1;

    /// @brief Emission coefficient
    const RGB ke = {0, 0, 0};

    /// @brief Emission probability
    const double pe = 0;

    /// @brief Construct a new Material object
    Material(const RGB& kd = {0, 0, 0}, double ks = 0, double kt = 0,
             double nt = 1, const RGB& ke = {0, 0, 0});

    /** 
     * @brief Creates a purely diffuse material.
     * @param kd Diffuse coefficient.
     * @return A diffuse Material.
     */
    static Material Diffuse(const RGB& kd);

    /** 
     * @brief Creates a purely specular material.
     * @param ks Specular coefficient.
     * @return A specular Material.
     */
    static Material Specular(double ks);

    /** 
     * @brief Creates a purely refractive material.
     * @param kt Refraction coefficient.
     * @param nt Refraction index.
     * @return A refractive Material.
     */
    static Material Refractive(double kt, double nt);
    
    /** 
     * @brief Creates a plastic material combining diffuse and specular components.
     * @param kd Diffuse coefficient.
     * @param ks Specular coefficient.
     * @return A plastic Material.
     */
    static Material Plastic(const RGB& kd, double ks);

    /** 
     * @brief Creates a dielectric material combining specular and refractive components.
     * @param ks Specular coefficient.
     * @param kt Refraction coefficient.
     * @param nt Refraction index.
     * @return A dielectric Material.
     */
    static Material Dielectric(double ks, double kt, double nt);

    /** 
     * @brief Creates a dielectric material combining diffuse, specular, and refractive components.
     * @param kd Diffuse coefficient.
     * @param ks Specular coefficient.
     * @param kt Refraction coefficient.
     * @param nt Refraction index.
     * @return A dielectric Material.
     */
    static Material Dielectric(const RGB& kd, double ks, double kt, double nt);

    /** 
     * @brief Creates a purely emissive material (light source).
     * @param ke Emission color coefficient.
     * @return An emissive Material.
     */
    static Material Emission(const RGB& ke);
};