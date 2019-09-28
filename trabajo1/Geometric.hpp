//******************************************************************************
// File:   Geometric.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Geometric objects
//******************************************************************************

#ifndef TRABAJO1_GEOMETRIC_HPP
#define TRABAJO1_GEOMETRIC_HPP

#include "HCoord.hpp"
#include "Transform.hpp"

// Elements

struct Sphere {
    HCoord center; // center of sphere (UCS)
    float radius; // radius of sphere (>=0)
};

struct Planet {
    Sphere sphere; // planet as sphere
    HCoord axisX; // local X axis (UCS)
    HCoord axisY; // local Y axis (UCS)
};

struct Station {
    HCoord position; // position of station (UCS)
    HCoord axisX; // local X axis (UCS)
    HCoord axisY; // local Y axis (UCS)
};

// Functions
Planet createPlanet(const HCoord &center, const HCoord &axis, const HCoord &city);

Station getStation(const Planet &planet, float inclination, float azimut);

HCoord getLocalDirection(const Station &local, const Station &other);

#endif //TRABAJO1_GEOMETRIC_HPP
