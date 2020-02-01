/******************************************************************************
 * @file    Texture.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#include <cmath>
#include "Texture.hpp"

using namespace std;

Texture colored(const Color &color) {
    return {
            .type = Texture::COLORED,
            .colorType = Texture::SOLID_COLOR,
            .color = {.solidColor = color}
    };
}

Texture colored(const VertexColor &color) {
    return Texture{
            .type = Texture::COLORED,
            .colorType = Texture::VERTEX_COLOR,
            .color = {.vertexColor = color}
    };
}

// 2D Textures
Texture sin2D(const Color &color) {
    return {
            .type = Texture::SIN_2D,
            .colorType = Texture::SOLID_COLOR,
            .color = {.solidColor = color}
    };
}

Texture sin2D(const VertexColor &color) {
    return Texture{
            .type = Texture::SIN_2D,
            .colorType = Texture::VERTEX_COLOR,
            .color = {.vertexColor = color}
    };
}

Texture sin22D(const Color &color) {
    return {
            .type = Texture::SIN2_2D,
            .colorType = Texture::SOLID_COLOR,
            .color = {.solidColor = color}
    };
}

Texture sin22D(const VertexColor &color) {
    return Texture{
            .type = Texture::SIN2_2D,
            .colorType = Texture::VERTEX_COLOR,
            .color = {.vertexColor = color}
    };
}

Texture sinCos2D(const Color &color) {
    return {
            .type = Texture::SIN_COS_2D,
            .colorType = Texture::SOLID_COLOR,
            .color = {.solidColor = color}
    };
}

Texture sinCos2D(const VertexColor &color) {
    return Texture{
            .type = Texture::SIN_COS_2D,
            .colorType = Texture::VERTEX_COLOR,
            .color = {.vertexColor = color}
    };
}

Color getColor(const Texture &texture, const HCoord &position) {
    float r, g, b;

    if (texture.colorType == Texture::SOLID_COLOR) {
        r = texture.color.solidColor.r;
        g = texture.color.solidColor.g;
        b = texture.color.solidColor.b;
    }
    else {
        Color c = getColor(texture.color.vertexColor, position);
        r = c.r;
        g = c.g;
        b = c.b;
    }

    switch (texture.type) {
        case Texture::SIN_2D: {
            return {
                    r * static_cast<float>(sin(position.x())),
                    g * static_cast<float>(sin(position.y())),
                    b * static_cast<float>(sin(position.z()))
            };
        }
        case Texture::SIN_COS_2D: {
            return {
                    r * static_cast<float>(sin(position.x()) * cos(position.x())),
                    g * static_cast<float>(sin(position.y()) * cos(position.y())),
                    b * static_cast<float>(sin(position.z()) * cos(position.z()))
            };
        }
        case Texture::SIN2_2D: {
            return {
                    r * static_cast<float>(sin(position.x() * position.y() * position.z())),
                    g * static_cast<float>(sin(position.x() * position.y() * position.z())),
                    b * static_cast<float>(sin(position.x() * position.y() * position.z()))
            };
        }
        default:
            return {
                r,
                g,
                b
            };
    }
}
