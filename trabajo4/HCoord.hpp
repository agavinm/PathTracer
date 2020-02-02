/******************************************************************************
 * @file    HCoord.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#ifndef TRABAJO1_HCOORD_HPP
#define TRABAJO1_HCOORD_HPP

#include <vector>
#include <ostream>

struct HCoord {
    float e[4]; // Coordinates

    float x() const; // x/w : +x goes inside the screen, -x goes outside the screen
    float y() const; // y/w : +y goes to the left, -y goes to the right
    float z() const; // z/w : +z goes to the top, -z goes to the bottom

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

    HCoord operator-() const;

    /**
     * Increases vector modulus by the amount 's'
     */
    HCoord operator*(float s) const;

    /**
     * @return the x,y,z element as a vector<float>
     */
    std::vector<float> as_vector() const;
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

// constants
static const HCoord P_ZERO = hPoint(0, 0, 0);
static const HCoord V_ZERO = hVector(0, 0, 0);
static const HCoord V_AX = hVector(1, 0, 0);
static const HCoord V_AY = hVector(0, 1, 0);
static const HCoord V_AZ = hVector(0, 0, 1);

#endif //TRABAJO1_HCOORD_HPP
