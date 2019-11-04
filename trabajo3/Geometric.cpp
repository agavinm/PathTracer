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
    cam.left = norm(cross(up, front)) * mod(up) * ratio;
    return cam;
}

GEOMETRY Sphere(const HCoord &center, float radius) {
    GEOMETRY geometry;
    geometry.type = SPHERE;
    geometry.type_data.sphere_data.radius = radius;
    geometry.type_data.sphere_data.center = center;
    return geometry;
}


MATERIAL Emitter(float r, float g, float b) {
    MATERIAL material;
    material.type = EMITTER;
    material.material_data.emitter_data.r = r;
    material.material_data.emitter_data.g = g;
    material.material_data.emitter_data.b = b;
    return material;
}


Object createPlane(const HCoord &normal, float dist) {
    return {};
}

HCoord getRay(Camera camera, float i, float j) {
    return camera.front + camera.up * (2 * i - 1) + camera.left * (2 * j - 1);
}

float posDist(const HCoord &point, const HCoord &dir, const Object &object) {

    switch (object.geometry.type) {
        case SPHERE: {
            GEOMETRY_SPHERE data = object.geometry.type_data.sphere_data;
            HCoord oc = point - data.center;
            float dotdoc = dot(norm(dir), oc);
            float _d = dotdoc * dotdoc - dot(oc, oc) + data.radius * data.radius;
            return _d < 0 ? INFINITY : -dotdoc - sqrt(_d);
        }
        case PLANE:
            break;
        default:
            exit(1);
    }

    return INFINITY;
}
