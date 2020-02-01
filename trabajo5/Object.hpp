/******************************************************************************
 * @file    Object.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#ifndef TRABAJO4_OBJECT_HPP
#define TRABAJO4_OBJECT_HPP

#include "Geometry.hpp"

enum OBJECT_TYPE {
    OBJECT_2D,
    OBJECT_3D,
    TRIANGULAR_PLY // Not real 3D objects
};

struct Object {
    const Geometry geometry;
    const Material material;
    const OBJECT_TYPE type;
    const float refractionRatio; // Refractive index (Snell's law)

    // Only for TRIANGULAR_PLY
    const std::vector<Object> triangles;
};

struct LightPoint {
    const Color color; // color of light
    const HCoord position; // position of light
};

LightPoint createLightPoint(const Color &color, const HCoord &position);

Object createObject(const Geometry &geometry, const Material &material);

Object createObject(const Geometry &geometry, const Material &material, float refractiveIndex);

/**
 * Triangular ply object is a triangle mesh without volume ("like a paper cover")
 * @param geometry is the coverage sphere
 * @param triangles
 * @return Triangular ply object
 */
Object createTRIANGULAR_PLY(const Geometry &geometry, const std::vector<Object> &triangles);

bool isInside(const HCoord &point, const Object &object);

/**
 * Calculates the intersection between an object and a ray
 * @param origin origin of ray
 * @param dir direction of ray
 * @param object the object to check colision
 * @return the distance between the origin and the intersection, or INFINITY if not collided
 */
float intersect(const HCoord &origin, const HCoord &dir, const Object &object);

std::pair<const Object *, float> intersect(const HCoord & origin, const HCoord &dir, const std::vector<Object> &objects);

std::pair<const Object *, float> triangularPlyIntersect(const HCoord &origin, const HCoord &dir, const Object &object);

const float VACUUM_REFRACTIVE_INDEX = 1.0f;
const float WATER_REFRACTIVE_INDEX = 1.330f;
const float AMBER_REFRACTIVE_INDEX = 1.55f;
const float LIQUID_HELIUM_REFRACTIVE_INDEX = 1.025f;

const float VACUUM_REFRACTIVE_RATIO = 1.0f / 1.0f;
const float WATER_REFRACTIVE_RATIO =  1.0f / 1.330f;
const float AMBER_REFRACTIVE_RATIO = 1.0f / 1.55f;
const float LIQUID_HELIUM_REFRACTIVE_RATIO = 1.0f / 1.025f;
#endif //TRABAJO4_OBJECT_HPP
