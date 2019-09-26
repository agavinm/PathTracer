//******************************************************************************
// File:   HCoord.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Points and Directions ADT
//******************************************************************************

#ifndef TRABAJO1_HCOORD_HPP
#define TRABAJO1_HCOORD_HPP

struct HCoord {
    float c[4]; // Coordinates
};

/*class HCoord {
    float c[4]; // Coordinates

public:
    HCoord(const float coordinates[4]) {
        c[0] = coordinates[0]; // x
        c[1] = coordinates[1]; // y
        c[2] = coordinates[2]; // z
        c[3] = coordinates[3]; // w : 1 = pont; 0 = direction
    }
};*/

#endif //TRABAJO1_HCOORD_HPP
