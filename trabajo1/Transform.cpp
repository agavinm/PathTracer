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
#include <cassert>

using namespace std;

Transform identity() {
    Transform m;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            m.e[i][j] = i == j ? 1.0f : 0.0f;
        }
    }

    return m;
}

Transform translation(const HCoord &t) {
    assert(!t.isVector());
    Transform m = identity();
    m.e[0][3] = t.x();
    m.e[1][3] = t.y();
    m.e[2][3] = t.z();

    return m;
}

Transform scale(const HCoord &s) {
    assert(!s.isVector());
    Transform m = identity();
    m.e[0][0] = s.x();
    m.e[1][1] = s.y();
    m.e[2][2] = s.z();

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
    assert(u.isVector() && v.isVector() && w.isVector());
    Transform m = identity();
    for (int i = 0; i < N; ++i) {
        m.e[i][0] = u.e[i];
        m.e[i][1] = v.e[i];
        m.e[i][2] = w.e[i];
        m.e[i][3] = o.e[i];
    }
    return m;
}

Transform inverse(const Transform &t) {
    //TODO

    return Transform();
}

// Operators

Transform &Transform::operator=(const Transform &other) {
    for (int row = 0; row < N; row++) {
        for (int column = 0; column < N; column++) {
            this->e[row][column] = other.e[row][column];
        }
    }
    return *this;
}

Transform Transform::operator*(const Transform &right) const {
    Transform result;
    for (int row = 0; row < N; row++) { // each row of result
        for (int column = 0; column < N; column++) { // each column of result
            result.e[row][column] = 0;
            for (int slice = 0; slice < N; slice++) { // traverse 'slice'
                result.e[row][column] += this->e[row][slice] * right.e[slice][column];
            }
        }
    }
    return result;
}

HCoord Transform::operator*(const HCoord &right) const {
    HCoord result;
    for (int column = 0; column < N; column++) { // each column on matrix
        result.e[column] = 0;
        for (int row = 0; row < N; row++) { // each row of matrix
            result.e[row] += this->e[row][column] * right.e[column];
        }
    }
    return result;
}
