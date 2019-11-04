//******************************************************************************
// File:   Geometric.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//         Geometric objects
//******************************************************************************

#ifndef TRABAJO1_GEOMETRIC_HPP
#define TRABAJO1_GEOMETRIC_HPP

#include "HCoord.hpp"
#include "Transform.hpp"

#include <array>

// Elements

enum GEOMETRY_TYPE {
    SPHERE,
    PLANE,
};

typedef struct {
    HCoord center; // center of sphere (UCS)
    float radius; // radius of sphere (>=0)
} GEOMETRY_SPHERE;

typedef struct {
    HCoord normal; // normal direction of plane (UCS)
    float dist; // distance to origin
} GEOMETRY_PLANE;

typedef struct {
    GEOMETRY_TYPE type;
    union {
        GEOMETRY_SPHERE sphere_data;
        GEOMETRY_PLANE plane_data;
    } type_data;
} GEOMETRY;


enum MATERIAL_TYPE {
    EMITTER
};

typedef struct {
    float r;
    float g;
    float b;
} MATERIAL_EMMITER;

typedef struct {
    MATERIAL_TYPE type;
    union {
        MATERIAL_EMMITER emitter_data;
    } material_data;
} MATERIAL;

struct Object {
    GEOMETRY geometry;
    MATERIAL material;
};

struct Camera {
    HCoord origin; // center of camera (UCS)
    HCoord front; // front vector of camera (UCS)
    HCoord left; // left vector of camera (UCS)
    HCoord up; // up vector of camera (UCS)
};

Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio);

Object createObject(const GEOMETRY &geometry, const MATERIAL &material);

GEOMETRY Sphere(const HCoord &center, float radius);

MATERIAL Emitter(float r, float g, float b);

Object createPlane(const HCoord &normal, float dist);

HCoord getRay(Camera camera, float i, float j);

float posDist(const HCoord &point, const HCoord &dir, const Object &object);

#endif //TRABAJO1_GEOMETRIC_HPP
