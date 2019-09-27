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

using namespace std;


HCoord zeroHCoord() {
    HCoord h;
    h.x = 0.0f;
    h.y = 0.0f;
    h.z = 0.0f;
    h.w = 0.0f;

    return h;
}

HCoord point(float x, float y, float z) {
    HCoord h;
    h.x = x;
    h.y = y;
    h.z = z;
    h.w = 1.0f;

    return h;
}

HCoord direction(float x, float y, float z) {
    HCoord h;
    h.x = x;
    h.y = y;
    h.z = z;
    h.w = 0.0f;

    return h;
}

float dot(const HCoord &a, const HCoord &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

HCoord cross(const HCoord &a, const HCoord &b) { // w component ignored
    HCoord h;
    h.x = a.y * b.z - a.z * b.y;
    h.y = a.z * b.x - a.x * b.z;
    h.z = a.x * b.y - a.y * b.x;
    //h.w = a.w * b.w;

    return h;
}

HCoord norm(const HCoord &h) {
    //TODO: La norma de un vector es un escalar
    return HCoord();
}

float mod(const HCoord &h) {
    return sqrt(h.x * h.x + h.y * h.y + h.z * h.z); // w component ignored
}

ostream& operator<<(ostream &o, const HCoord &h) {
    o << "(" << h.x << ", " << h.y << ", " << h.z << ", " << h.w << ")";

    return o;
}

HCoord &HCoord::operator=(const HCoord &h) {
    this->x = h.x;
    this->y = h.y;
    this->z = h.z;
    this->w = h.w;

    return *this;
}

bool HCoord::operator==(const HCoord &h) const {
    return this->x == h.x && this->y == h.y && this->z == h.z && this->w == h.w;
}

HCoord HCoord::operator+(const HCoord &h) const { // w component ignored
    HCoord v;
    v.x = this->x + h.x;
    v.y = this->y + h.y;
    v.z = this->z + h.z;
    //v.w = this->w * h.w;

    return v;
}

HCoord HCoord::operator-(const HCoord &h) const {
    HCoord v;
    v.x = this->x + h.x;
    v.y = this->y + h.y;
    v.z = this->z + h.z;
    //v.w = this->w * h.w;

    return v;
}
