/******************************************************************************
 * @file    Texture.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include <tuple>
#include "Texture.hpp"
#include "Geometric.hpp"

using namespace std;

// 2D Textures
Texture sin2D(const COLOR &color) {
    return {
            .type = Texture::SIN_2D,
            .color = color
    };
}

Texture sin22D(const COLOR &color) {
    return {
            .type = Texture::SIN2_2D,
            .color = color
    };
}

Texture sinCos2D(const COLOR &color) {
    return {
            .type = Texture::SIN_COS_2D,
            .color = color
    };
}

Texture vertexColor(const COLOR colors[3], const HCoord vertices[3]) {
    return {
        .type = Texture::VERTEX_COLOR,
        .colors = {colors[0], colors[1], colors[2]},
        .vertices = {vertices[0], vertices[1], vertices[2]}
    };
}

COLOR getColor(const Texture &texture, const HCoord &position) {
    switch (texture.type) {
        case Texture::SIN_2D: {
            return {
                texture.color.r * static_cast<float>(sin(position.x())),
                texture.color.g * static_cast<float>(sin(position.y())),
                texture.color.b * static_cast<float>(sin(position.z()))
            };
        }
        case Texture::SIN_COS_2D: {
            return {
                texture.color.r * static_cast<float>(sin(position.x()) * cos(position.x())),
                texture.color.g * static_cast<float>(sin(position.y()) * cos(position.y())),
                texture.color.b * static_cast<float>(sin(position.z()) * cos(position.z()))
            };
        }
        case Texture::SIN2_2D: {
            return {
                texture.color.r * static_cast<float>(sin(position.x() * position.y() * position.z())),
                texture.color.g * static_cast<float>(sin(position.x() * position.y() * position.z())),
                texture.color.b * static_cast<float>(sin(position.x() * position.y() * position.z()))
            };
        }
        case Texture::VERTEX_COLOR: {
            float mods[3] = {mod(position - texture.vertices[0]),
                             mod(position - texture.vertices[1]),
                             mod(position - texture.vertices[2])};

            if (mods[0] == 0.0f) return texture.colors[0];
            else if (mods[1] == 0.0f) return texture.colors[1];
            else if (mods[2] == 0.0f) return texture.colors[2];

            return (oposite(texture.colors[0]) * mods[0] + oposite(texture.colors[1]) * mods[1] +
                    oposite(texture.colors[2]) * mods[2]) / (mods[0] + mods[1] + mods[2]);
        }
        default:
            return texture.color;
    }
}
