/******************************************************************************
 * @file    Texture.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include "Texture.hpp"

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

COLOR getColor2D(const Texture &texture, const int i, const int j) {
    switch (texture.type) {
        case Texture::SIN_2D: {
            return {
                texture.color.r * static_cast<float>(sin(i)),
                texture.color.g * static_cast<float>(sin(i)),
                texture.color.b * static_cast<float>(sin(i))
            };
        }
        case Texture::SIN_COS_2D: {
            return {
                texture.color.r * static_cast<float>(sin(i)*cos(j)),
                texture.color.g * static_cast<float>(sin(i)*cos(j)),
                texture.color.b * static_cast<float>(sin(i)*cos(j))
            };
        }
        case Texture::SIN2_2D: {
            return {
                texture.color.r * static_cast<float>(sin(i*j)),
                texture.color.g * static_cast<float>(sin(i*j)),
                texture.color.b * static_cast<float>(sin(i*j))
            };
        }
        default:
            return texture.color;
    }
}
