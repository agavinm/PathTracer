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

Material Delta(const Texture &kd, const Texture &ks) {
    return Material {
            .type = DELTA,
            .property = {.reflectance = {.kd = kd, .ks = ks}}
    };
}

Material Diffuse(const Texture &kd) {
    return Material {
            .type = DELTA,
            .property = {.reflectance = {.kd = kd, .ks = colored(C_BLACK)}}
    };
}

Material Phong(const Texture &kd, const Texture &ks) {
    return Material {
            .type = PHONG,
            .property = {.reflectance = {.kd = kd, .ks = ks}}
    };
}