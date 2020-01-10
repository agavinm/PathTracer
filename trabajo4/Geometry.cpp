/******************************************************************************
 * @file    Geometry.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include "Geometry.hpp"

Geometry Sphere(const HCoord &center, float radius) {
    return {
            .type=SPHERE,
            .data={.sphere={
                    .center=center,
                    .radius=radius,
            }},
    };
}

Geometry Plane(const HCoord &normal, float dist) {
    return {
            .type = PLANE,
            .data={.plane={
                    .normal = norm(normal),
                    .dist = dist,
            }},
    };
}

Geometry Triangle(const HCoord &point1, const HCoord &point2, const HCoord &point3) {
    HCoord dirX = point2 - point1;
    HCoord dirY = point3 - point1;
    HCoord normal = norm(cross(dirX, dirY));
    float dist = dot(P_ZERO - point1, normal);
    return Geometry{
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

Geometry Circle(const HCoord &center, const HCoord &axisX, const HCoord &axisY) {
    HCoord normal = norm(cross(axisX, axisY));
    float dist = dot(P_ZERO - center, normal);

    return Geometry{
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

Geometry Cuadric(float A, float B, float C, float D, float E, float F, float G, float H, float I, float J) {
    return Geometry{
            .type = CUADRIC,
            .data = {.cuadric = {
                    A, B, C, D, E, F, G, H, I, J
            }},
    };
}

HCoord normal(const Geometry &geometry, const HCoord &position) {
    switch (geometry.type) {
        case SPHERE: {
            return norm(position - geometry.data.sphere.center);
        }
        case PLANE: {
            return geometry.data.plane.normal;
        }
        case TRIANGLE: {
            return geometry.data.triangle.plane.normal;
        }
        case CIRCLE: {
            return geometry.data.circle.plane.normal;
        }
        case CUADRIC: {
            GEOMETRY_CUADRIC data = geometry.data.cuadric;

            //http://skuld.bmsc.washington.edu/people/merritt/graphics/quadrics.html

            float xn = 2*data.A*position.x() + data.D*position.y() + data.E*position.z() + data.G;
            float yn = 2*data.B*position.y() + data.D*position.x() + data.F*position.z() + data.H;
            float zn = 2*data.C*position.z() + data.E*position.x() + data.F*position.y() + data.I;
            return norm(hVector(xn, yn, zn));
        }
    }
}
