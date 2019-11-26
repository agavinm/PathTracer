/******************************************************************************
 * @file    Material.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO3_MATERIAL_HPP
#define TRABAJO3_MATERIAL_HPP

#include "Image.hpp"
#include "Texture.hpp"

// Materials

enum MATERIAL_TYPE {
    EMITTER
};

typedef struct {
    const MATERIAL_TYPE type;
    const Texture texture;
} Material;

/**
 * Creates a textured emmiter material
 * @return the textured emmiter as material
 */
Material Emitter(const Texture &texture);

#endif //TRABAJO3_MATERIAL_HPP
