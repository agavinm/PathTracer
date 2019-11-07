//******************************************************************************
// File:   HCoord.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Points and Directions ADT
//******************************************************************************

#ifndef TRABAJO1_HCOORD_HPP
#define TRABAJO1_HCOORD_HPP

#include <ostream>


struct HCoord {
    const float e[4]; // Coordinates

    float x() const; // x/w
    float y() const; // y/w
    float z() const; // z/w

    bool isVector() const; // w!=0

    /**
     * same vector or same points
     */
    bool operator==(const HCoord &h) const;

    /**
     * vector + vector = vector
     * vector + point = point
     * point + vector = point
     * point + point = ERROR
     */
    HCoord operator+(const HCoord &right) const;

    /**
     * vector - vector = vector
     * vector - point = ERROR
     * point - vector = point
     * point - point = vector
     */
    HCoord operator-(const HCoord &right) const;

    /**
     * Increases vector modulus by the amount 's'
     */
    HCoord operator*(float s) const;
};

/**
 * A point in space (UCS)
 */
HCoord hPoint(float x, float y, float z);

/**
 * A vector in space
 */
HCoord hVector(float x, float y, float z);

/**
 * Dot product of two vectors
 */
float dot(const HCoord &a, const HCoord &b);

/**
 * Cross product of two vectors
 */
HCoord cross(const HCoord &a, const HCoord &b);

/**
 * Normalizes a vector
 */
HCoord norm(const HCoord &h);

/**
 * Modulus of a vector
 */
float mod(const HCoord &h);

/**
 * To pretty print a point/vector
 */
std::ostream &operator<<(std::ostream &o, const HCoord &h);


#endif //TRABAJO1_HCOORD_HPP
