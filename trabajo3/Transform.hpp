//******************************************************************************
// File:   Transform.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Transform Matrix ADT
//******************************************************************************

#ifndef TRABAJO1_TRANSFORM_HPP
#define TRABAJO1_TRANSFORM_HPP

#include "HCoord.hpp"

// Interface:

struct Transform;

Transform translation(const HCoord &t);

Transform scale(const HCoord &s);

Transform rotationX(float th);

Transform rotationY(float th);

Transform rotationZ(float th);

Transform changeFromBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o);

Transform changeToBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o);

Transform inverse(const Transform &t);


// Declarations:

struct Transform {
    const float e[4][4]; // Elements (row, column)

    /**
     * Multiplication matrix * matrix -> matrix
     */
    Transform operator*(const Transform &right) const;

    /**
     * Multiplication matrix * vector -> vector
     */
    HCoord operator*(const HCoord &right) const;
};

#endif //TRABAJO1_TRANSFORM_HPP
