//******************************************************************************
// File:   HPoint.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         TAD que representa puntos y direcciones
//******************************************************************************

#ifndef TRABAJO1_HPOINT_HPP
#define TRABAJO1_HPOINT_HPP

class HPoint {
    float c[4]; // Coordenadas

public:
    HPoint(const float coordinates[4]) {
        c[0] = coordinates[0]; // x
        c[1] = coordinates[1]; // y
        c[2] = coordinates[2]; // z
        c[3] = coordinates[3]; // w : 1 = punto; 0 = dirección
    }
};

#endif //TRABAJO1_HPOINT_HPP
