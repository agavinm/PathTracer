/******************************************************************************
 * @file    Geometry.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO1_GEOMETRIC_HPP
#define TRABAJO1_GEOMETRIC_HPP

#include <array>
#include "HCoord.hpp"
#include "Material.hpp"

// Geometries

enum GEOMETRY_TYPE {
    SPHERE,
    PLANE,
    TRIANGLE,
    CIRCLE,
    CUADRIC
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
    const float A, B, C, D, E, F, G, H, I, J; // cuadric values
} GEOMETRY_CUADRIC;

typedef struct {
    const GEOMETRY_TYPE type;
    const union {
        const GEOMETRY_SPHERE sphere;
        const GEOMETRY_PLANE plane;
        const GEOMETRY_TRIANGLE triangle;
        const GEOMETRY_CIRCLE circle;
        const GEOMETRY_CUADRIC cuadric;
    } data;
} Geometry;

/**
 * Creates a sphere
 * @param center center of sphere (UCS)
 * @param radius radious of sphere (UCS)
 * @return the sphere as geometry
 */
Geometry Sphere(const HCoord &center, float radius);

/**
 * Creates a plane
 * @param normal normal vector of plane
 * @param dist distance to origin (in normal direction: if normal is (0,0,1) and dist=1 the plane is on (0,0,-1) visible from origin)
 * @return the plane as geometry
 */
Geometry Plane(const HCoord &normal, float dist);

/**
 * Creates a triangle from 3 points. The normal of the triangle is cross(point2-point1,point3-point1)
 * @param point1 a point, like the 'center' of the triangle
 * @param point2 like the end of the axisX of the triangle
 * @param point3 like the end of the axisY of the triangle
 * @return the triangle as geometry
 */
Geometry Triangle(const HCoord &point1, const HCoord &point2, const HCoord &point3);

/**
 * Creates a circle (Oval) from a center and two axis
 * @param center center of the circle
 * @param axisX direction of the x axis, its length marks the end of the circle
 * @param axisY direction of the y axis, its length marks the end of the circle
 * @return the circle as geometry
 */
Geometry Circle(const HCoord &center, const HCoord &axisX, const HCoord &axisY);

/**
 * Creates a circle (Oval) from a center and two axis
 * @param center center of the circle
 * @param axisX direction of the x axis, its length marks the end of the circle
 * @param axisY direction of the y axis, its length marks the end of the circle
 * @return the circle as geometry
 */
Geometry Cuadric(float A, float B, float C, float D, float E, float F, float G, float H, float I, float J);

// Objects

struct Object {
    const Geometry geometry;
    const Material material;
};

/**
 * Calculates the intersection between an object and a ray
 * @param origin origin of ray
 * @param dir direction of ray
 * @param object the object to check colision
 * @return the distance between the origin and the intersection, or INFINITY if not collided
 */
float intersect(const HCoord &origin, const HCoord &dir, const Object &object);

#endif //TRABAJO1_GEOMETRIC_HPP
