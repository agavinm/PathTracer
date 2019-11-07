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

//            // Unoptimized
//            HCoord normdir = norm(dir);
//            HCoord ominusc = origin - data.center; // origin minus center
//            float a = 1;
//            float b = 2 * dot(normdir, ominusc);
//            float c = dot(ominusc, ominusc) - data.radius * data.radius;
//            float discriminant = b * b - 4 * a * c;
//            if (discriminant < 1e-6)
//                return INFINITY;
//            float t = (-b + sqrt(discriminant)) / 2 / a;
//            return t > -1e-6 ? t : INFINITY;

            // Optimized
            HCoord ominusc = origin - data.center; // origin minus center
            float b = dot(norm(dir), ominusc);
            float discriminant = b * b - dot(ominusc, ominusc) + data.radius * data.radius;
            return discriminant < 1e-6 ? INFINITY : -b - sqrt(discriminant);

        }
        case PLANE: {
            GEOMETRY_PLANE data = object.geometry.data.plane;

            HCoord normdir = norm(dir);
            float denom = dot(normdir, norm(data.normal));
            return denom > -1e-6 ? INFINITY : -(dot(origin - hPoint(0, 0, 0), normdir) + data.dist) / denom;

        }
        default:
            exit(50);
    }
}
