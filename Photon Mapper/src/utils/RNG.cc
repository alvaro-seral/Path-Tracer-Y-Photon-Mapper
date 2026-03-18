/**
 * @file RNG.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "RNG.hh"

#include <random>

std::mt19937 r(std::random_device {}());

double uniform(const double min, const double max)
{
    std::uniform_real_distribution<double> my_uniform(min, max);
    return min + (max - min) * my_uniform(r);
}