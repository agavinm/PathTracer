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

Material Reflector(const Texture &kd, const Texture &ks, const float kp) {
    return Material {
            .type = REFLECTOR,
            .property = {.reflectance = {.kd = kd, .ks = ks, .kp = kp}}
    };
}