/******************************************************************************
 * @file    Transform.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

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

// Constants
static const float EPS = 1e-6;

#define SIMILAR(a,b) ( ((a) < (b) + EPS) && ((a) > (b) - EPS))


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
