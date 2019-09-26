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


// Interface:

struct Transform;

Transform translation(float t[3]);

Transform scale(float s[3]);

Transform rotationX(float th);

Transform rotationY(float th);

Transform rotationZ(float th);

Transform changeBase(float u[3], float v[3], float w[3], float o[3]);

Transform inverse(Transform t);


// Declarations:

struct Transform {
    friend Transform translation(float t[3]);
    friend Transform scale(float s[3]);
    friend Transform rotationX(float th);
    friend Transform rotationY(float th);
    friend Transform rotationZ(float th);
    friend Transform changeBase(float u[3], float v[3], float w[3], float o[3]);
    friend Transform inverse(Transform t);

private:
    float e[4][4]; // Elements (row, column)

    friend Transform identity();
};

#endif //TRABAJO1_TRANSFORM_HPP
