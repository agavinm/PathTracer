/******************************************************************************
 * @file    HCoord.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include "HCoord.hpp"
#include <cmath>
#include <cassert>

using namespace std;


HCoord hPoint(float x, float y, float z) {
    return {{x, y, z, 1.0f}};
}

HCoord hVector(float x, float y, float z) {
    return {{x, y, z, 0.0f}};
}

float dot(const HCoord &a, const HCoord &b) {
    assert(a.isVector() && b.isVector());

    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

HCoord cross(const HCoord &a, const HCoord &b) {
    assert(a.isVector() && b.isVector());

    return {{
                    a.e[1] * b.e[2] - a.e[2] * b.e[1],
                    a.e[2] * b.e[0] - a.e[0] * b.e[2],
                    a.e[0] * b.e[1] - a.e[1] * b.e[0],
                    0
            }};
}

HCoord norm(const HCoord &h) {
    assert(h.isVector());

    float modh = mod(h);
    return {{
                    h.e[0] / modh,
                    h.e[1] / modh,
                    h.e[2] / modh,
                    0
            }};
}

float mod(const HCoord &h) {
    assert(h.isVector());
    return sqrt(h.e[0] * h.e[0] + h.e[1] * h.e[1] + h.e[2] * h.e[2]);
}

ostream &operator<<(ostream &o, const HCoord &h) {
    if (h.isVector()) {
        o << "Vector(" << h.e[0] << ", " << h.e[1] << ", " << h.e[2] << ")";
    } else {
        o << "Point(" << h.x() << ", " << h.y() << ", " << h.z() << ")";
    }
    return o;
}

bool HCoord::operator==(const HCoord &h) const {
    // check if one is vector and the other isn't
    if (h.isVector() != this->isVector()) return false;

    if (h.isVector()) {
        // two vectors, no third component
        return this->e[0] == h.e[0] && this->e[1] == h.e[1] && this->e[2] == h.e[2];
    } else {
        // two points, same coordinates
        return this->x() == h.x() && this->y() == h.y() && this->z() == h.z();
    }
}

HCoord HCoord::operator+(const HCoord &right) const {
    if (this->isVector()) {
        if (right.isVector()) {
            // vector + vector = vector
            return {{
                            this->e[0] + right.e[0],
                            this->e[1] + right.e[1],
                            this->e[2] + right.e[2],
                            0
                    }};
        } else {
            // vector + point = point
            float w = right.e[3];
            return {{
                            this->e[0] * w + right.e[0],
                            this->e[1] * w + right.e[1],
                            this->e[2] * w + right.e[2],
                            w
                    }};
        }
    } else {
        if (right.isVector()) {
            // point + vector = point
            float w = this->e[3];
            return {{
                            this->e[0] + right.e[0] * w,
                            this->e[1] + right.e[1] * w,
                            this->e[2] + right.e[2] * w,
                            w
                    }};
        } else {
            // point + point = ERROR
            assert(false);
        }
    }
}

HCoord HCoord::operator-(const HCoord &right) const {
    if (this->isVector()) {
        if (right.isVector()) {
            // vector - vector = vector
            return {{
                            this->e[0] - right.e[0],
                            this->e[1] - right.e[1],
                            this->e[2] - right.e[2],
                            0
                    }};
        } else {
            // vector - point = ERROR
            assert(false);
        }
    } else {
        if (right.isVector()) {
            // point - vector = point
            float w = this->e[3];
            return {{
                            this->e[0] - right.e[0] * w,
                            this->e[1] - right.e[1] * w,
                            this->e[2] - right.e[2] * w,
                            w
                    }};
        } else {
            // point - point = vector
            return {{
                            this->x() - right.x(),
                            this->y() - right.y(),
                            this->z() - right.z(),
                            0
                    }};
        }
    }
}

HCoord HCoord::operator-() const {
    return {{-this->e[0], -this->e[1], -this->e[2], this->e[3]}};
}

HCoord HCoord::operator*(float s) const {
    assert(this->isVector());

    return {{
                    this->e[0] * s,
                    this->e[1] * s,
                    this->e[2] * s,
                    0
            }};
}

float HCoord::x() const {
    return this->isVector() ? this->e[0] : this->e[0] / this->e[3];
}

float HCoord::y() const {
    return this->isVector() ? this->e[1] : this->e[1] / this->e[3];
}

float HCoord::z() const {
    return this->isVector() ? this->e[2] : this->e[2] / this->e[3];
}

bool HCoord::isVector() const {
    return this->e[3] == 0;
}
