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


// Interface:

struct HCoord;

HCoord dot(HCoord a, HCoord b);

HCoord cross(HCoord a, HCoord b);


// Declarations:

struct HCoord {
    HCoord operator+(const HCoord &h);
    HCoord operator-(const HCoord &h);
    HCoord operator*(float f);


private:
    float c[4]; // Coordinates
};

#endif //TRABAJO1_HCOORD_HPP
