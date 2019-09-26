//******************************************************************************
// File:   Transform.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Transform Matrix ADT
//******************************************************************************

#include "Transform.hpp"
#include <cmath>

using namespace std;

Transform identity() {
    Transform m;
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            m.e[i][j] = 0.0f;
        }
    }
    m.e[0][0] = 1.0f;
    m.e[1][1] = 1.0f;
    m.e[2][2] = 1.0f;
    m.e[3][3] = 1.0f;

    return m;
}

Transform translation(float t[3]) {
    Transform m = identity();
    m.e[0][3] = t[0];
    m.e[1][3] = t[1];
    m.e[2][3] = t[2];

    return m;
}

Transform scale(float s[3]) {
    Transform m = identity();
    m.e[0][0] = s[0];
    m.e[1][1] = s[1];
    m.e[2][2] = s[2];

    return m;
}

Transform rotationX(float th) {
    Transform m = identity();
    m.e[1][1] = cos(th);
    m.e[2][1] = sin(th);
    m.e[1][2] = -m.e[2][1];
    m.e[2][2] = m.e[1][1];

    return m;
}

Transform rotationY(float th) {
    Transform m = identity();
    m.e[0][0] = cos(th);
    m.e[0][2] = sin(th);
    m.e[2][0] = -m.e[0][2];
    m.e[2][2] = m.e[0][0];

    return m;
}

Transform rotationZ(float th) {
    Transform m = identity();
    m.e[0][0] = cos(th);
    m.e[1][0] = sin(th);
    m.e[0][1] = -m.e[1][0];
    m.e[1][1] = m.e[0][0];

    return m;
}

Transform changeBase(float u[3], float v[3], float w[3], float o[3]) {
    Transform m = identity();
    m.e[0][0] = u[0];
    m.e[1][0] = u[1];
    m.e[2][0] = u[2];
    m.e[0][1] = v[0];
    m.e[1][1] = v[1];
    m.e[2][1] = v[2];
    m.e[0][2] = w[0];
    m.e[1][2] = w[1];
    m.e[2][2] = w[2];
    m.e[0][3] = o[0];
    m.e[1][3] = o[1];
    m.e[2][3] = o[2];
    return m;
}

Transform inverse(Transform t) {
    Transform m = identity();
    //TODO

    return m;
}
