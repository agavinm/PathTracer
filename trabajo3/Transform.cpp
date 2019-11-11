/******************************************************************************
 * @file    Transform.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include "Transform.hpp"
#include <cmath>
#include <cassert>

using namespace std;

Transform identity() {
    return {{
                    {1, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
            }};
}

Transform translation(const HCoord &t) {
    assert(!t.isVector());
    return {{
                    {1, 0, 0, t.x()},
                    {0, 1, 0, t.y()},
                    {0, 0, 1, t.z()},
                    {0, 0, 0, 1}
            }};
}

Transform scale(const HCoord &s) {
    assert(!s.isVector());
    return {{
                    {s.x(), 0, 0, 0},
                    {0, s.y(), 0, 0},
                    {0, 0, s.z(), 0},
                    {0, 0, 0, 1}
            }};
}

Transform rotationX(float th) {
    float c = cos(th);
    float s = sin(th);

    return {{
                    {1, 0, 0, 0},
                    {0, c, -s, 0},
                    {0, s, c, 0},
                    {0, 0, 0, 1}
            }};
}

Transform rotationY(float th) {
    float c = cos(th);
    float s = sin(th);

    return {{
                    {c, 0, s, 0},
                    {0, 1, 0, 0},
                    {-s, 0, c, 0},
                    {0, 0, 0, 1}
            }};
}

Transform rotationZ(float th) {
    float c = cos(th);
    float s = sin(th);

    return {{
                    {c, -s, 0, 0},
                    {s, c, 0, 0},
                    {0, 0, 1, 0},
                    {0, 0, 0, 1}
            }};
}

Transform changeFromBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o) {
    assert(u.isVector() && v.isVector() && w.isVector());
    return {{
                    {u.e[0], v.e[0], w.e[0], o.e[0]},
                    {u.e[1], v.e[1], w.e[1], o.e[1]},
                    {u.e[2], v.e[2], w.e[2], o.e[2]},
                    {u.e[3], v.e[3], w.e[3], o.e[3]},
            }};
}

Transform changeToBase(const HCoord &u, const HCoord &v, const HCoord &w, const HCoord &o) {
    return inverse(changeFromBase(u, v, w, o));
}

Transform inverse(const Transform &t) {

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

    return {{
                    {
                            det * (t.e[1][1] * A2323 - t.e[1][2] * A1323 + t.e[1][3] * A1223),
                            det * -(t.e[0][1] * A2323 - t.e[0][2] * A1323 + t.e[0][3] * A1223),
                            det * (t.e[0][1] * A2313 - t.e[0][2] * A1313 + t.e[0][3] * A1213),
                            det * -(t.e[0][1] * A2312 - t.e[0][2] * A1312 + t.e[0][3] * A1212),
                    },
                    {
                            det * -(t.e[1][0] * A2323 - t.e[1][2] * A0323 + t.e[1][3] * A0223),
                            det * (t.e[0][0] * A2323 - t.e[0][2] * A0323 + t.e[0][3] * A0223),
                            det * -(t.e[0][0] * A2313 - t.e[0][2] * A0313 + t.e[0][3] * A0213),
                            det * (t.e[0][0] * A2312 - t.e[0][2] * A0312 + t.e[0][3] * A0212),
                    },
                    {
                            det * (t.e[1][0] * A1323 - t.e[1][1] * A0323 + t.e[1][3] * A0123),
                            det * -(t.e[0][0] * A1323 - t.e[0][1] * A0323 + t.e[0][3] * A0123),
                            det * (t.e[0][0] * A1313 - t.e[0][1] * A0313 + t.e[0][3] * A0113),
                            det * -(t.e[0][0] * A1312 - t.e[0][1] * A0312 + t.e[0][3] * A0112),
                    },
                    {
                            det * -(t.e[1][0] * A1223 - t.e[1][1] * A0223 + t.e[1][2] * A0123),
                            det * (t.e[0][0] * A1223 - t.e[0][1] * A0223 + t.e[0][2] * A0123),
                            det * -(t.e[0][0] * A1213 - t.e[0][1] * A0213 + t.e[0][2] * A0113),
                            det * (t.e[0][0] * A1212 - t.e[0][1] * A0212 + t.e[0][2] * A0112),
                    }
            }};
}

// Operators

Transform Transform::operator*(const Transform &right) const { // matrix * matrix
    float r[4][4] = {0};
    for (int row = 0; row < 4; row++) { // each row of result
        for (int column = 0; column < 4; column++) { // each column of result
            r[row][column] = 0;
            for (int slice = 0; slice < 4; slice++) { // traverse 'slice'
                r[row][column] += this->e[row][slice] * right.e[slice][column];
            }
        }
    }
    return {{
                    {r[0][0], r[0][1], r[0][2], r[0][3]},
                    {r[1][0], r[1][1], r[1][2], r[1][3]},
                    {r[2][0], r[2][1], r[2][2], r[2][3]},
                    {r[3][0], r[3][1], r[3][2], r[3][3]},
            }};
}

HCoord Transform::operator*(const HCoord &right) const { // matrix * vector
    float r[4] = {0};
    for (int row = 0; row < 4; row++) { // each row of matrix
        r[row] = 0;
        for (int column = 0; column < 4; column++) { // each column on matrix
            r[row] += this->e[row][column] * right.e[column];
        }
    }
    return {{r[0], r[1], r[2], r[3]}};
}
