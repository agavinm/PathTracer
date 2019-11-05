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
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
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

Transform changeFromBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o) {
    assert(u.isVector() && v.isVector() && w.isVector());
    Transform m = identity();
    for (int i = 0; i < 3; ++i) {
        m.e[i][0] = u.e[i];
        m.e[i][1] = v.e[i];
        m.e[i][2] = w.e[i];
        m.e[i][3] = o.e[i];
    }
    return m;
}

Transform changeToBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o) {
    return inverse(changeFromBase(u, v, w, o));
}

Transform inverse(const Transform &t) {
    Transform result;

    //https://stackoverflow.com/a/44446912
    float A2323 = t.e[2][2] * t.e[3][3] - t.e[2][3] * t.e[3][2];
    float A1323 = t.e[2][1] * t.e[3][3] - t.e[2][3] * t.e[3][1];
    float A1223 = t.e[2][1] * t.e[3][2] - t.e[2][2] * t.e[3][1];
    float A0323 = t.e[2][0] * t.e[3][3] - t.e[2][3] * t.e[3][0];
    float A0223 = t.e[2][0] * t.e[3][2] - t.e[2][2] * t.e[3][0];
    float A0123 = t.e[2][0] * t.e[3][1] - t.e[2][1] * t.e[3][0];
    float A2313 = t.e[1][2] * t.e[3][3] - t.e[1][3] * t.e[3][2];
    float A1313 = t.e[1][1] * t.e[3][3] - t.e[1][3] * t.e[3][1];
    float A1213 = t.e[1][1] * t.e[3][2] - t.e[1][2] * t.e[3][1];
    float A2312 = t.e[1][2] * t.e[2][3] - t.e[1][3] * t.e[2][2];
    float A1312 = t.e[1][1] * t.e[2][3] - t.e[1][3] * t.e[2][1];
    float A1212 = t.e[1][1] * t.e[2][2] - t.e[1][2] * t.e[2][1];
    float A0313 = t.e[1][0] * t.e[3][3] - t.e[1][3] * t.e[3][0];
    float A0213 = t.e[1][0] * t.e[3][2] - t.e[1][2] * t.e[3][0];
    float A0312 = t.e[1][0] * t.e[2][3] - t.e[1][3] * t.e[2][0];
    float A0212 = t.e[1][0] * t.e[2][2] - t.e[1][2] * t.e[2][0];
    float A0113 = t.e[1][0] * t.e[3][1] - t.e[1][1] * t.e[3][0];
    float A0112 = t.e[1][0] * t.e[2][1] - t.e[1][1] * t.e[2][0];

    float det = t.e[0][0] * (t.e[1][1] * A2323 - t.e[1][2] * A1323 + t.e[1][3] * A1223)
                - t.e[0][1] * (t.e[1][0] * A2323 - t.e[1][2] * A0323 + t.e[1][3] * A0223)
                + t.e[0][2] * (t.e[1][0] * A1323 - t.e[1][1] * A0323 + t.e[1][3] * A0123)
                - t.e[0][3] * (t.e[1][0] * A1223 - t.e[1][1] * A0223 + t.e[1][2] * A0123);
    det = 1 / det;

    result.e[0][0] = det * (t.e[1][1] * A2323 - t.e[1][2] * A1323 + t.e[1][3] * A1223);
    result.e[0][1] = det * -(t.e[0][1] * A2323 - t.e[0][2] * A1323 + t.e[0][3] * A1223);
    result.e[0][2] = det * (t.e[0][1] * A2313 - t.e[0][2] * A1313 + t.e[0][3] * A1213);
    result.e[0][3] = det * -(t.e[0][1] * A2312 - t.e[0][2] * A1312 + t.e[0][3] * A1212);
    result.e[1][0] = det * -(t.e[1][0] * A2323 - t.e[1][2] * A0323 + t.e[1][3] * A0223);
    result.e[1][1] = det * (t.e[0][0] * A2323 - t.e[0][2] * A0323 + t.e[0][3] * A0223);
    result.e[1][2] = det * -(t.e[0][0] * A2313 - t.e[0][2] * A0313 + t.e[0][3] * A0213);
    result.e[1][3] = det * (t.e[0][0] * A2312 - t.e[0][2] * A0312 + t.e[0][3] * A0212);
    result.e[2][0] = det * (t.e[1][0] * A1323 - t.e[1][1] * A0323 + t.e[1][3] * A0123);
    result.e[2][1] = det * -(t.e[0][0] * A1323 - t.e[0][1] * A0323 + t.e[0][3] * A0123);
    result.e[2][2] = det * (t.e[0][0] * A1313 - t.e[0][1] * A0313 + t.e[0][3] * A0113);
    result.e[2][3] = det * -(t.e[0][0] * A1312 - t.e[0][1] * A0312 + t.e[0][3] * A0112);
    result.e[3][0] = det * -(t.e[1][0] * A1223 - t.e[1][1] * A0223 + t.e[1][2] * A0123);
    result.e[3][1] = det * (t.e[0][0] * A1223 - t.e[0][1] * A0223 + t.e[0][2] * A0123);
    result.e[3][2] = det * -(t.e[0][0] * A1213 - t.e[0][1] * A0213 + t.e[0][2] * A0113);
    result.e[3][3] = det * (t.e[0][0] * A1212 - t.e[0][1] * A0212 + t.e[0][2] * A0112);

    return result;
}

// Operators

Transform Transform::operator*(const Transform &right) const { // matrix * matrix
    Transform result;
    for (int row = 0; row < 4; row++) { // each row of result
        for (int column = 0; column < 4; column++) { // each column of result
            result.e[row][column] = 0;
            for (int slice = 0; slice < 4; slice++) { // traverse 'slice'
                result.e[row][column] += this->e[row][slice] * right.e[slice][column];
            }
        }
    }
    return result;
}

HCoord Transform::operator*(const HCoord &right) const { // matrix * vector
    HCoord result;
    for (int row = 0; row < 4; row++) { // each row of matrix
        result.e[row] = 0;
        for (int column = 0; column < 4; column++) { // each column on matrix
            result.e[row] += this->e[row][column] * right.e[column];
        }
    }
    return result;
}
