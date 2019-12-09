/******************************************************************************
 * @file    Object.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include "Object.hpp"
#include "Transform.hpp"
#include <cmath>
#include <cassert>

using namespace std;

Object create3D(const Geometry &geometry, const Material &material, float refractiveIndex) {
    assert(geometry.type == SPHERE);

    return {
            .geometry = geometry,
            .material = material,
            .type = OBJECT_3D,
            .n = refractiveIndex
    };
}

Object create2D(const Geometry &geometry, const Material &material) {
    assert(geometry.type != SPHERE);

    return {
        .geometry = geometry,
        .material = material,
        .type = OBJECT_2D,
        .n = VACUUM_REFRACTIVE_INDEX
    };
}

LightPoint createLightPoint(const Color &color, const HCoord &position) {
    return {.color = color,.position = position};
}

bool isInside(const HCoord &point, const Object &object) {
    assert(object.type == OBJECT_3D);
    switch (object.geometry.type) {
        case SPHERE: {
            return mod(point - object.geometry.data.sphere.center) <= object.geometry.data.sphere.radius;
        }
        default: {
            exit(6);
        }
    }
}


pair<const Object *, float> intersect(const HCoord &origin, const HCoord &dir, const vector<Object> &objects) {
    const Object *intersection = nullptr;
    float dist = INFINITY;
    for (const Object &object : objects) {
        float obj_dist = intersect(origin, dir, object);
        if (obj_dist > EPS && obj_dist < dist) {
            intersection = &object;
            dist = obj_dist;
        }
    }
    return {intersection, dist};
}

float intersect(const HCoord &origin, const HCoord &dir, const Object &object) {
    assert(mod(dir) < 1 + EPS && mod(dir) > 1 - EPS);
    switch (object.geometry.type) {
        case SPHERE: {
            GEOMETRY_SPHERE data = object.geometry.data.sphere;

//            // Unoptimized
//            HCoord ominusc = origin - data.center; // origin minus center
//            float a = 1;
//            float b = 2 * dot(dir, ominusc);
//            float c = dot(ominusc, ominusc) - data.radius * data.radius;
//            float discriminant = b * b - 4 * a * c;
//            if (discriminant < EPS)
//                return INFINITY;
//            float t1 = (-b + sqrt(discriminant)) / 2 / a;
//            float t2 = (-b - sqrt(discriminant)) / 2 / a;
//            if(t1<EPS) return t2;
//            if(t2<EPS) return t1;
//            return t1 < t2 ? t1 : t2;

            // Optimized
            HCoord ominusc = origin - data.center; // origin minus center
            float b = dot(dir, ominusc);
            float discriminant = b * b - dot(ominusc, ominusc) + data.radius * data.radius;
            if (discriminant < EPS) return INFINITY;
            float t1 = -b + sqrt(discriminant);
            float t2 = -b - sqrt(discriminant);
            return t1 < EPS ? t2 :
                   t2 < EPS ? t1 :
                   t1 < t2 ? t1 :
                   t2;

        }
        case PLANE: {
            GEOMETRY_PLANE data = object.geometry.data.plane;

            float denom = dot(dir, data.normal);
            return denom == 0 ? INFINITY : -(dot(origin - P_ZERO, data.normal) + data.dist) / denom;
        }
        case TRIANGLE: {
            GEOMETRY_TRIANGLE data = object.geometry.data.triangle;

            float denom = dot(dir, data.plane.normal);
            if (denom == 0)
                return INFINITY;

            float dist = -(dot(origin - P_ZERO, data.plane.normal) + data.plane.dist) / denom;
            HCoord point = changeToBase(data.dirX, data.dirY, data.plane.normal, data.point) * (origin + dir * dist);

            if (point.x() < 0 || point.y() < 0 || point.x() + point.y() > 1)
                return INFINITY;
            return dist;
        }
        case CIRCLE: {
            GEOMETRY_CIRCLE data = object.geometry.data.circle;

            float denom = dot(dir, data.plane.normal);
            if (denom == 0)
                return INFINITY;

            float dist = -(dot(origin - P_ZERO, data.plane.normal) + data.plane.dist) / denom;
            HCoord point = changeToBase(data.axisX, data.axisY, data.plane.normal, data.center) * (origin + dir * dist);

            if (point.x() * point.x() + point.y() * point.y() > 1 + EPS)
                return INFINITY;
            return dist;
        }
        case CUADRIC: {
            GEOMETRY_CUADRIC data = object.geometry.data.cuadric;

            //http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html
            float Aq = data.A * dir.x() * dir.x() + data.B * dir.y() * dir.y() + data.C * dir.z() * dir.z() + data.D * dir.x() * dir.y() + data.E * dir.x() * dir.z() + data.F * dir.y() * dir.z();

            float Bq = 2 * data.A * origin.x() * dir.x() + 2 * data.B * origin.y() * dir.y() + 2 * data.C * origin.z() * dir.z() + data.D * (origin.x() * dir.y() + origin.y() * dir.x()) + data.E * (origin.x() * dir.z() + origin.z() * dir.x()) + data.F * (origin.y() * dir.z() + dir.y() * origin.z()) + data.G * dir.x() + data.H * dir.y() + data.I * dir.z();

            float Cq = data.A * origin.x() * origin.x() + data.B * origin.y() * origin.y() + data.C * origin.z() * origin.z() + data.D * origin.x() * origin.y() + data.E * origin.x() * origin.z() + data.F * origin.y() * origin.z() + data.G * origin.x() + data.H * origin.y() + data.I * origin.z() + data.J;

            if (Aq == 0) {
                return -Cq / Bq;
            }

            float discr = Bq * Bq - 4 * Aq * Cq;
            if (discr < 0) {
                return INFINITY;
            }

            float t0 = (-Bq - sqrt(discr)) / (2 * Aq);

            if (t0 > 0) return t0;

            float t1 = (-Bq + sqrt(discr)) / 2 * Aq;

            return t1;

        }
        default:
            exit(50);
    }
}
