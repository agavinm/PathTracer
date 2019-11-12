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

struct Texture {
private:
    enum TEXTURE_TYPE {
        // 2D types hardcoded:
        SIN_2D,
        SIN2_2D,
        SIN_COS_2D,

        // 2D types from file:
        IMAGE_2D
    };

public:
    const TEXTURE_TYPE type;
    const COLOR color;

    friend Texture sin2D(const COLOR &color);
    friend Texture sin22D(const COLOR &color);
    friend Texture sinCos2D(const COLOR &color);
    friend COLOR getColor2D(const Texture &texture, int i, int j);
};

Texture sin2D(const COLOR &color);

Texture sin22D(const COLOR &color);

Texture sinCos2D(const COLOR &color);

COLOR getColor2D(const Texture &texture, int i, int j);

#endif //TRABAJO3_TEXTURE_HPP
