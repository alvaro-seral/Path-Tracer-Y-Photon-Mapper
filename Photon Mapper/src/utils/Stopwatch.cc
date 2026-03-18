/**
 * @file Stopwatch.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Stopwatch.hh"

void Stopwatch::start() { t1 = std::chrono::high_resolution_clock::now(); }

void Stopwatch::stop() { t2 = std::chrono::high_resolution_clock::now(); }

std::string Stopwatch::toString() const
{
    std::chrono::minutes m
        = std::chrono::duration_cast<std::chrono::minutes>(t2 - t1);
    std::chrono::seconds s
        = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1) % 60;
    std::chrono::milliseconds ms
        = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1) % 1000;

    return std::to_string(m.count()) + "m " + std::to_string(s.count()) + "s "
        + std::to_string(ms.count()) + "ms";
}