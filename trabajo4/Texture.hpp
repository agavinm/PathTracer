/******************************************************************************
 * @file    Texture.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO3_TEXTURE_HPP
#define TRABAJO3_TEXTURE_HPP

#include "Color.hpp"

typedef struct {
    enum TEXTURE_TYPE {
        // Only color without any texture:
        COLORED,

        // 2D types:
        SIN_2D,
        SIN2_2D,
        SIN_COS_2D,

        // 2D types from file:
        IMAGE_2D
    };

    enum COLOR_TYPE {
        SOLID_COLOR,
        VERTEX_COLOR
    };

    const TEXTURE_TYPE type;
    const COLOR_TYPE colorType;

    const union {
        const Color solidColor;
        const VertexColor vertexColor;
    } color;
} Texture;

Texture colored(const Color &color);
Texture colored(const VertexColor &color);

Texture sin2D(const Color &color);
Texture sin2D(const VertexColor &color);

Texture sin22D(const Color &color);
Texture sin22D(const VertexColor &color);

Texture sinCos2D(const Color &color);
Texture sinCos2D(const VertexColor &color);

Color getColor(const Texture &texture, const HCoord &position);

#endif //TRABAJO3_TEXTURE_HPP
