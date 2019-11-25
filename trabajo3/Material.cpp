/******************************************************************************
 * @file    Material.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include "Material.hpp"

Material Emitter(const Texture &texture) {
    return Material {
            .type = EMITTER,
            .texture = texture
    };
}