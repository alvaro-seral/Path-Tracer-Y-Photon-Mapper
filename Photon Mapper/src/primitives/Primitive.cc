/**
 * @file Primitive.cc
 * @author Dorian Boleslaw Wozniak (817570@unizar.es)
 * @author Álvaro Seral Gracia (819425@unizar.es)
 */

#include "Primitive.hh"
#include "utils/Logger.hh"

#include <cassert>

Primitive::Primitive(const Material& material) : material(material) { 
    id = ID_COUNTER;
    ID_COUNTER++;
}

Primitive::~Primitive() { }

unsigned int Primitive::getId() { return id; }