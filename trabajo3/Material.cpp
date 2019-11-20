/******************************************************************************
 * @file    Material.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include <cassert>
#include "Material.hpp"
#include "Transform.hpp"
#include "Geometric.hpp"

MATERIAL Emitter(const COLOR &color) {
    return {
            .type = EMITTER,
            .data={.emitter = color},
    };
}

MATERIAL Texturer(const Texture &texture) {
    return MATERIAL{
            .type = TEXTURE,
            .data={.texture = texture},
    };
}

float intersect(const HCoord &origin, const HCoord &dir, const Object &object) {
    assert(mod(dir) < 1 + EPS && mod(dir) > 1 - EPS);
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
//            if (discriminant < EPS)
//                return INFINITY;
//            float t = (-b + sqrt(discriminant)) / 2 / a;
//            return t > -EPS ? t : INFINITY;

            // Optimized
            HCoord ominusc = origin - data.center; // origin minus center
            float b = dot(dir, ominusc);
            float discriminant = b * b - dot(ominusc, ominusc) + data.radius * data.radius;
            return discriminant < EPS ? INFINITY : -b - sqrt(discriminant);

        }
        case PLANE: {
            GEOMETRY_PLANE data = object.geometry.data.plane;

            float denom = dot(dir, data.normal);
            return denom > -EPS ? INFINITY : -(dot(origin - P_ZERO, dir) + data.dist) / denom;

        }
        case TRIANGLE: {
            GEOMETRY_TRIANGLE data = object.geometry.data.triangle;

            float denom = dot(dir, data.plane.normal);
            if (denom == 0)
                return INFINITY;

            float dist = -(dot(origin - P_ZERO, dir) + data.plane.dist) / denom;
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

            float dist = -(dot(origin - P_ZERO, dir) + data.plane.dist) / denom;
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

            float Cq = data.A * origin.x() * 2 + data.B * origin.y() * 2 + data.C * origin.z() * 2 + data.D * origin.x() * origin.y() + data.E * origin.x() * origin.z() + data.F * origin.y() * origin.z() + data.G * origin.x() + data.H * origin.y() + data.I * origin.z() + data.J;

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