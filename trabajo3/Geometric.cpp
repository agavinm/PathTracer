/******************************************************************************
 * @file    Geometric.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include "Geometric.hpp"

GEOMETRY Sphere(const HCoord &center, float radius) {
    return {
            .type=SPHERE,
            .data={.sphere={
                    .center=center,
                    .radius=radius,
            }},
    };
}

GEOMETRY Plane(const HCoord &normal, float dist) {
    return {
            .type = PLANE,
            .data={.plane={
                    .normal = norm(normal),
                    .dist = dist,
            }},
    };
}

GEOMETRY Triangle(const HCoord &point1, const HCoord &point2, const HCoord &point3) {
    HCoord dirX = point2 - point1;
    HCoord dirY = point3 - point1;
    HCoord normal = norm(cross(dirX, dirY));
    float dist = dot(P_ZERO - point1, normal);
    return GEOMETRY{
            .type = TRIANGLE,
            .data = {.triangle = {
                    .plane = {
                            .normal = normal,
                            .dist = dist,
                    },
                    .point = point1,
                    .dirX = dirX,
                    .dirY = dirY,
            }},
    };
}

GEOMETRY Circle(const HCoord &center, const HCoord &axisX, const HCoord &axisY) {
    HCoord normal = norm(cross(axisX, axisY));
    float dist = dot(P_ZERO - center, normal);

    return GEOMETRY{
            .type = CIRCLE,
            .data = {.circle = {
                    .plane = {
                            .normal = normal,
                            .dist = dist,
                    },
                    .center = center,
                    .axisX = axisX,
                    .axisY = axisY,
            }},
    };
}

GEOMETRY Cuadric(float A, float B, float C, float D, float E, float F, float G, float H, float I, float J) {
    return GEOMETRY{
            .type = CUADRIC,
            .data = {.cuadric = {
                    A, B, C, D, E, F, G, H, I, J
            }},
    };
}
