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
#include "Image.hpp"

#include <array>

// Geometries

enum GEOMETRY_TYPE {
    SPHERE,
    PLANE,
    TRIANGLE,
    CIRCLE,
};

typedef struct {
    const HCoord center; // center of sphere (UCS)
    const float radius; // radius of sphere (>=0)
} GEOMETRY_SPHERE;

typedef struct {
    const HCoord normal; // normal direction of plane (UCS, normalized)
    const float dist; // distance to origin
} GEOMETRY_PLANE;

typedef struct {
    const GEOMETRY_PLANE plane; // plane this triangle is on. The normal is cross(dirX,dirY)
    const HCoord point; // first point
    const HCoord dirX; // second point
    const HCoord dirY; // third point
} GEOMETRY_TRIANGLE;

typedef struct {
    const GEOMETRY_PLANE plane; // plane this circle is on. THe normal is cross(axisX,axisY)
    const HCoord center; // center of circle
    const HCoord axisX; // X axis, defines the max value
    const HCoord axisY; // Y axis, defines the max value
} GEOMETRY_CIRCLE;

typedef struct {
    const GEOMETRY_TYPE type;
    const union {
        const GEOMETRY_SPHERE sphere;
        const GEOMETRY_PLANE plane;
        const GEOMETRY_TRIANGLE triangle;
        const GEOMETRY_CIRCLE circle;
    } data;
} GEOMETRY;

/**
 * Creates a sphere
 * @param center center of sphere (UCS)
 * @param radius radious of sphere (UCS)
 * @return the sphere as geometry
 */
GEOMETRY Sphere(const HCoord &center, float radius);

/**
 * Creates a plane
 * @param normal normal vector of plane
 * @param dist distance to origin (in normal direction: if normal is (0,0,1) and dist=1 the plane is on (0,0,-1) visible from origin)
 * @return the plane as geometry
 */
GEOMETRY Plane(const HCoord &normal, float dist);

/**
 * Creates a triangle from 3 points. The normal of the triangle is cross(point2-point1,point3-point1)
 * @param point1 a point, like the 'center' of the triangle
 * @param point2 like the end of the axisX of the triangle
 * @param point3 like the end of the axisY of the triangle
 * @return the triangle as geometry
 */
GEOMETRY Triangle(const HCoord &point1, const HCoord &point2, const HCoord &point3);

/**
 * Creates a circle (Oval) from a center and two axis
 * @param center center of the circle
 * @param axisX direction of the x axis, its length marks the end of the circle
 * @param axisY direction of the y axis, its length marks the end of the circle
 * @return the circle as geometry
 */
GEOMETRY Circle(const HCoord &center, const HCoord &axisX, const HCoord &axisY);

// Materials

enum MATERIAL_TYPE {
    EMITTER
};

typedef struct {
    const MATERIAL_TYPE type;
    const union {
        const COLOR emitter;
    } data;
} MATERIAL;

/**
 * Creates an emitter material
 * @return the emitter as material
 */
MATERIAL Emitter(COLOR color);

// Objects

struct Object {
    const GEOMETRY geometry;
    const MATERIAL material;
};

// Camera

struct Camera {
    const HCoord origin; // center of camera (UCS)
    const HCoord front; // front vector of camera (UCS)
    const HCoord left; // left vector of camera (UCS)
    const HCoord up; // up vector of camera (UCS)
};

/**
 * Creates a camera
 * @param origin point (UCS)
 * @param front direction from origin to center of image (UCS)
 * @param up direction from center of image to top side of image
 * @param ratio width/height of image
 * @return the camera
 */
Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio);

/**
 * Computes the direction of a ray from a camera
 * @param camera origin of ray
 * @param i horizontal position in image (0=left -> 1=right)
 * @param j vertical position in image (0=up -> 1=bottom)
 * @return the ray as vector
 */
HCoord getRay(Camera camera, float i, float j);

/**
 * Calculates the intersection between an object and a ray
 * @param origin origin of ray
 * @param dir direction of ray
 * @param object the object to check colision
 * @return the distance between the origin and the intersection, or INFINITY if not collided
 */
float intersect(const HCoord &origin, const HCoord &dir, const Object &object);

#endif //TRABAJO1_GEOMETRIC_HPP
