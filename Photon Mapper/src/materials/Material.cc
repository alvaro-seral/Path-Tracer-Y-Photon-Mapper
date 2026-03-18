#include "Material.hh"

#include <cassert>

Material::Material(const RGB& kd, double ks, double kt, double nt,
                   const RGB& ke) :
    kd(kd),
    pd(kd.max()),
    ks(RGB(ks, ks, ks)),
    ps(ks),
    kt(RGB(kt, kt, kt)),
    pt(kt),
    nt(nt),
    ke(ke),
    pe(ke.max())
{
    // Probabilities and their sum must be in [0,1)
    assert(0 <= pd && pd < 1);
    assert(0 <= ps && ps < 1);
    assert(0 <= pt && pt < 1);
    assert(pd + ps + pt < 1);

    // Either fully emissive or not
    assert(!pe || (pe && (!pd && !ps && !pt)));
}

Material Material::Diffuse(const RGB& kd) { 
    return Material(kd); 
}

Material Material::Specular(double ks) { 
    return Material({0, 0, 0}, ks); 
}

Material Material::Refractive(double kt, double nt)
{
    return Material({0, 0, 0}, 0, kt, nt);
}

Material Material::Plastic(const RGB& kd, double ks)
{
    return Material(kd, ks);
}

Material Material::Dielectric(double ks, double kt, double nt)
{
    return Material({0, 0, 0}, ks, kt, nt);
}

Material Material::Dielectric(const RGB& kd, double ks, double kt, double nt)
{
    return Material(kd, ks, kt, nt);
}

Material Material::Emission(const RGB& ke) { 
    return Material({0, 0, 0}, 0, 0, 1, ke);
}
