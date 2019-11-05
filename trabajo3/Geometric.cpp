//******************************************************************************
// File:   Geometric.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//******************************************************************************

#include <cmath>
#include "Geometric.hpp"

Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio) {
    Camera cam;
    cam.origin = origin;
    cam.front = front;
    cam.up = up;
    cam.left = norm(cross(up, front)) * mod(up) * ratio; // left is ortogonal to up and left, with the size of up*ratio
    return cam;
}

GEOMETRY Sphere(const HCoord &center, float radius) {
    GEOMETRY geometry;
    geometry.type = SPHERE;
    geometry.data.sphere.radius = radius;
    geometry.data.sphere.center = center;
    return geometry;
}

GEOMETRY Plane(const HCoord &normal, float dist) {
    GEOMETRY geometry;
    geometry.type = PLANE;
    geometry.data.plane.normal = normal;
    geometry.data.plane.dist = dist;
    return geometry;
}

MATERIAL Emitter(COLOR color) {
    MATERIAL material;
    material.type = EMITTER;
    material.data.emitter = color;
    return material;
}

HCoord getRay(Camera camera, float i, float j) {
    return camera.front + camera.left * (2 * i - 1) + camera.up * (2 * j - 1);
}

float intersect(const HCoord &origin, const HCoord &dir, const Object &object) {

    switch (object.geometry.type) {
        case SPHERE: {
            GEOMETRY_SPHERE data = object.geometry.data.sphere;
            // this needs understanding and comments
            HCoord oc = origin - data.center;
            float dotdoc = dot(norm(dir), oc);
            float _d = dotdoc * dotdoc - dot(oc, oc) + data.radius * data.radius;
            return _d < 1e-6 ? INFINITY : -dotdoc - sqrt(_d);
        }
        case PLANE: {
            GEOMETRY_PLANE data = object.geometry.data.plane;
            // this needs understanding and comments
            float denom = dot(norm(data.normal), norm(dir));
            if (fabs(denom) > 1e-6) {
                HCoord p0l0 = (hPoint(0, 0, 0) + data.normal * data.dist) - origin;
                float t = dot(p0l0, norm(data.normal)) / denom;
                return t > 1e-6 ? t : INFINITY;
            }
            return INFINITY;
        }
        default:
            exit(50);
    }

    return INFINITY;
}
