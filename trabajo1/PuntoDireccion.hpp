//******************************************************************************
// File:   PuntoDireccion.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         TAD que representa puntos y direcciones
//******************************************************************************

#ifndef TRABAJO1_PUNTODIRECCION_HPP
#define TRABAJO1_PUNTODIRECCION_HPP

class PuntoDireccion {
    float c[4]; // Coordenadas

public:
    PuntoDireccion(const float coordenadas[4]) {
        c[0] = coordenadas[0]; // x
        c[1] = coordenadas[1]; // y
        c[2] = coordenadas[2]; // z
        c[3] = coordenadas[3]; // w : 1 = punto; 0 = dirección
    }
};

#endif //TRABAJO1_PUNTODIRECCION_HPP
