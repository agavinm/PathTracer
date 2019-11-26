/******************************************************************************
 * @file    Material.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include "Material.hpp"

Material Emitter(const Texture &texture) {
    return Material {
            .type = EMITTER,
            .property = {.texture = texture}
    };
}

Material Reflecter(const Texture &kd, const Texture &ks) {
    return Material {
            .type = REFLECTER,
            .property = {.k = {.kd = kd, .ks = ks}}
    };
}

Material Brdf(const Texture &kd, const Texture &ks) {
    return Material {
            .type = BRDF,
            .property = {.k = {.kd = kd, .ks = ks}}
    };
}