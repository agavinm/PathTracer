/******************************************************************************
 * @file    Texture.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO3_TEXTURE_HPP
#define TRABAJO3_TEXTURE_HPP

#include "Image.hpp"
#include "HCoord.hpp"
#include "Geometric.hpp"

struct Texture {
private:
    enum TEXTURE_TYPE {
        // 2D types hardcoded:
        SIN_2D,
        SIN2_2D,
        SIN_COS_2D,

        // 2D types from file:
        IMAGE_2D,

        VERTEX_COLOR
    };

public:
    const TEXTURE_TYPE type;
    const union {
        const COLOR color;
        const COLOR colors[3];
    };
    const HCoord vertices[3];

    friend Texture sin2D(const COLOR &color);
    friend Texture sin22D(const COLOR &color);
    friend Texture sinCos2D(const COLOR &color);
    friend Texture vertexColor(const COLOR colors[3], const HCoord vertices[3]);

    friend COLOR getColor(const Texture &texture, const HCoord &position);
};

Texture sin2D(const COLOR &color);

Texture sin22D(const COLOR &color);

Texture sinCos2D(const COLOR &color);

Texture vertexColor(const COLOR colors[3], const HCoord vertices[3]);

COLOR getColor(const Texture &texture, const HCoord &position);

#endif //TRABAJO3_TEXTURE_HPP
