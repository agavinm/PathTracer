/******************************************************************************
 * @file    Object.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO4_OBJECT_HPP
#define TRABAJO4_OBJECT_HPP

#include "Geometry.hpp"

enum OBJECT_TYPE {
    OBJECT_2D,
    OBJECT_3D
};

struct Object {
    const Geometry geometry;
    const Material material;
    const OBJECT_TYPE type;
    const float n; // Refractive index (Snell's law)
};

Object create2D(const Geometry &geometry, const Material &material);

Object create3D(const Geometry &geometry, const Material &material, float refractiveIndex);

bool isInside(const HCoord &point, const Object &object);

/**
 * Calculates the intersection between an object and a ray
 * @param origin origin of ray
 * @param dir direction of ray
 * @param object the object to check colision
 * @return the distance between the origin and the intersection, or INFINITY if not collided
 */
float intersect(const HCoord &origin, const HCoord &dir, const Object &object);

const float VACUUM_REFRACTIVE_INDEX = 1.0f;
const float WATER_REFRACTIVE_INDEX = 1.330f;
const float AMBER_REFRACTIVE_INDEX = 1.55f;
const float LIQUID_HELIUM_REFRACTIVE_INDEX = 1.025;
#endif //TRABAJO4_OBJECT_HPP
