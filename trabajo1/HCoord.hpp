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

#include <ostream>


// Interface:

struct HCoord;

HCoord zeroHCoord();

HCoord point(float x, float y, float z);

HCoord direction(float x, float y, float z);

HCoord dot(const HCoord &a, const HCoord &b);

HCoord cross(const HCoord &a, const HCoord &b);

HCoord norm(const HCoord &h);

float mod(const HCoord &h);

std::ostream& operator<<(std::ostream &o, const HCoord &h);


// Declarations:

struct HCoord {
    float e[4]; // Coordinates

    float x() const; // x/w
    float y() const; // y/w
    float z() const; // z/w

    bool isVector() const; // w!=0

    HCoord& operator=(const HCoord &h);
    bool operator==(const HCoord &h) const;
    HCoord operator+(const HCoord &h) const;
    HCoord operator-(const HCoord &h) const;
};

#endif //TRABAJO1_HCOORD_HPP
