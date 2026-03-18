/**
 * @file Photon.hh
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "geometry/Point.hh"
#include "geometry/Direction.hh"
#include "image/RGB.hh"
#include "kdtree.h"

/** @brief Photon class representing a photon in the photon map.
 * 
 * A photon is defined by its position, incident direction, flux, and an ID.
 */
class Photon 
{
public:
    Point position;
    Direction incident_dir;
    RGB flux;
    unsigned int id;

    Photon(Point p, Direction d, RGB f, unsigned int id) 
        : position(p), incident_dir(d), flux(f), id(id) {}
};

/// @brief PhotonAxisPosition class used to access the position of a photon in the KD-tree.
struct PhotonAxisPosition 
{
	double operator()(const Photon& p, std::size_t i) const {
        switch (i) {
            case 0: return p.position.x;
            case 1: return p.position.y;
            case 2: return p.position.z;
            default: 
                throw std::out_of_range("Fotón fuera de rango");
        }
	}
};

/** @brief PhotonMap type definition using KDTree with Photon as the 
 *  element type and PhotonAxisPosition as the axis position accessor.
 */
using PhotonMap = nn::KDTree<Photon,3,PhotonAxisPosition>;
