/******************************************************************************
 * @file    Material.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include "Material.hpp"
#include "Transform.hpp"

MATERIAL Emitter(const COLOR &color) {
    return {
            .type = EMITTER,
            .data={.emitter = color},
    };
}

MATERIAL Texturer(const Texture &texture) {
    return {
            .type = TEXTURE,
            .data={.texture = texture},
    };
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
//            if (discriminant < EPS)
//                return INFINITY;
//            float t = (-b + sqrt(discriminant)) / 2 / a;
//            return t > -EPS ? t : INFINITY;

            // Optimized
            HCoord ominusc = origin - data.center; // origin minus center
            float b = dot(norm(dir), ominusc);
            float discriminant = b * b - dot(ominusc, ominusc) + data.radius * data.radius;
            return discriminant < EPS ? INFINITY : -b - sqrt(discriminant);

        }
        case PLANE: {
            GEOMETRY_PLANE data = object.geometry.data.plane;

            HCoord normdir = norm(dir);
            float denom = dot(normdir, data.normal);
            return denom > -EPS ? INFINITY : -(dot(origin - P_ZERO, normdir) + data.dist) / denom;

        }
        case TRIANGLE: {
            GEOMETRY_TRIANGLE data = object.geometry.data.triangle;

            HCoord normdir = norm(dir);
            float denom = dot(normdir, data.plane.normal);
            if (denom == 0)
                return INFINITY;

            float dist = -(dot(origin - P_ZERO, normdir) + data.plane.dist) / denom;
            HCoord point = changeToBase(data.dirX, data.dirY, data.plane.normal, data.point) * (origin + normdir * dist);

            if (point.x() < 0 || point.y() < 0 || point.x() + point.y() > 1)
                return INFINITY;
            return dist;
        }
        case CIRCLE: {
            GEOMETRY_CIRCLE data = object.geometry.data.circle;

            HCoord normdir = norm(dir);
            float denom = dot(normdir, data.plane.normal);
            if (denom == 0)
                return INFINITY;

            float dist = -(dot(origin - P_ZERO, normdir) + data.plane.dist) / denom;
            HCoord point = changeToBase(data.axisX, data.axisY, data.plane.normal, data.center) * (origin + normdir * dist);

            if (point.x() * point.x() + point.y() * point.y() > 1 + EPS)
                return INFINITY;
            return dist;
        }
        default:
            exit(50);
    }
}