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

Transform translation(const HCoord &t) {
    Transform m = identity();
    m.e[0][3] = t.x;
    m.e[1][3] = t.y;
    m.e[2][3] = t.z;

    return m;
}

Transform scale(const HCoord &s) {
    Transform m = identity();
    m.e[0][0] = s.x;
    m.e[1][1] = s.y;
    m.e[2][2] = s.z;

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

Transform changeBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o) {
    Transform m = identity();
    m.e[0][0] = u.x;
    m.e[1][0] = u.y;
    m.e[2][0] = u.z;
    m.e[0][1] = v.x;
    m.e[1][1] = v.y;
    m.e[2][1] = v.z;
    m.e[0][2] = w.x;
    m.e[1][2] = w.y;
    m.e[2][2] = w.z;
    m.e[0][3] = o.x;
    m.e[1][3] = o.y;
    m.e[2][3] = o.z;
    return m;
}

Transform inverse(const Transform &t) {
    //TODO

    return Transform();
}

// Operators

Transform &Transform::operator=(const Transform &t) {
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            this->e[i][j] = t.e[i][j];
        }
    }

    return *this;
}

Transform Transform::operator*(const Transform &t) const {
    //TODO

    return Transform();
}

HCoord Transform::operator*(const HCoord &t) const {
    //TODO

    return HCoord();
}
