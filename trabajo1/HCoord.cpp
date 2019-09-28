//******************************************************************************
// File:   HCoord.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Points and Directions ADT
//******************************************************************************

#include "HCoord.hpp"
#include <cmath>
#include <cassert>

using namespace std;


HCoord point(float x, float y, float z) {
    HCoord h;
    h.e[0] = x;
    h.e[1] = y;
    h.e[2] = z;
    h.e[3] = 1.0f;
    return h;
}

HCoord vector(float x, float y, float z) {
    HCoord h;
    h.e[0] = x;
    h.e[1] = y;
    h.e[2] = z;
    h.e[3] = 0.0f;
    return h;
}

float dot(const HCoord &a, const HCoord &b) {
    assert(a.isVector() && b.isVector());

    return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}

HCoord cross(const HCoord &a, const HCoord &b) {
    assert(a.isVector() && b.isVector());

    HCoord h;
    h.e[0] = a.e[1] * b.e[2] - a.e[2] * b.e[1];
    h.e[1] = a.e[2] * b.e[0] - a.e[0] * b.e[2];
    h.e[2] = a.e[0] * b.e[1] - a.e[1] * b.e[0];
    h.e[3] = 0;
    return h;
}

HCoord norm(const HCoord &h) {
    assert(h.isVector());

    HCoord n;
    for (int i = 0; i < 3; i++) {
        n.e[i] = h.e[i] / mod(h);
    }
    n.e[3] = 0;
    return n;
}

float mod(const HCoord &h) {
    assert(h.isVector());
    return sqrt(h.e[0] * h.e[0] + h.e[1] * h.e[1] + h.e[2] * h.e[2]);
}

HCoord vectorify(const HCoord &p) {
    assert(!p.isVector());

    HCoord v = p;
    v.e[3] = 0;
    return v;
}

ostream &operator<<(ostream &o, const HCoord &h) {
    if (h.isVector()) {
        o << "Vector(" << h.e[0] << ", " << h.e[1] << ", " << h.e[2] << ")";
    } else {
        o << "Point(" << h.x() << ", " << h.y() << ", " << h.z() << ")";
    }
    return o;
}

HCoord &HCoord::operator=(const HCoord &h) {
    for (int i = 0; i < 4; i++) {
        this->e[i] = h.e[i];
    }
    return *this;
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
    HCoord v;
    if (this->isVector()) {
        if (right.isVector()) {
            // vector + vector
            for (int i = 0; i < 3; ++i) {
                v.e[i] = this->e[i] + right.e[i];
            }
            v.e[3] = 0;
        } else {
            // vector + point
            for (int i = 0; i < 3; ++i) {
                v.e[i] = this->e[i] * right.e[3] + right.e[i];
            }
            v.e[3] = right.e[3];
        }
    } else {
        if (right.isVector()) {
            // point + vector
            for (int i = 0; i < 3; ++i) {
                v.e[i] = this->e[i] + right.e[i] * this->e[3];
            }
            v.e[3] = this->e[3];
        } else {
            // point + point
            assert(false);
        }
    }
    return v;
}

HCoord HCoord::operator-(const HCoord &right) const {
    HCoord v;
    if (this->isVector()) {
        if (right.isVector()) {
            // vector - vector
            for (int i = 0; i < 3; ++i) {
                v.e[i] = this->e[i] - right.e[i];
            }
            v.e[3] = 0;
        } else {
            // vector - point
            assert(false);
        }
    } else {
        if (right.isVector()) {
            // point - vector
            for (int i = 0; i < 3; ++i) {
                v.e[i] = this->e[i] - right.e[i] * this->e[3];
            }
            v.e[3] = this->e[3];
        } else {
            // point - point
            float w = this->e[3] * right.e[3];
            for (int i = 0; i < 3; ++i) {
                v.e[i] = (this->e[i] * right.e[3] - right.e[i] * this->e[3]) / w;
            }
            v.e[3] = 0;
        }
    }
    return v;
}

float HCoord::x() const {
    assert(!this->isVector());

    return this->e[0] / this->e[3];
}

float HCoord::y() const {
    assert(!this->isVector());

    return this->e[1] / this->e[3];
}

float HCoord::z() const {
    assert(!this->isVector());

    return this->e[2] / this->e[3];
}

bool HCoord::isVector() const {
    return this->e[3] == 0;
}
