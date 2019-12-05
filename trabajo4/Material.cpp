/******************************************************************************
 * @file    Material.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include "Material.hpp"

Material Emitter(const Texture &texture) {
    return Material {
            .type = EMITTER,
            .property = {.texture = texture}
    };
}

Material Reflector(const Texture &kd, const Texture &ks, const float s, const Texture &kdPhong, const Texture &ksPhong) {
    return Material {
            .type = REFLECTOR,
            .property = {.reflectance = {.kd = kd, .ks = ks, .kdPhong = kdPhong, .ksPhong = ksPhong, .s = s}}
    };
}

Material Diffuse(const Texture &k) {
    return Reflector(colored(C_BLACK), colored(C_BLACK), 1, k, k);
}

Material Phong(const Texture &kdPhong, const Texture &ksPhong, const float s) {
    return Reflector(colored(C_BLACK), colored(C_BLACK), s, kdPhong, ksPhong);
}

Material Delta(const Texture &kd, const Texture &ks, const float s) {
    return Reflector(kd, ks, s, colored(C_BLACK), colored(C_BLACK));
}

Material Refractor(const Texture &kd) {
    return Reflector(kd, colored(C_BLACK), 1, colored(C_BLACK), colored(C_BLACK));
}

Material Specular(const Texture &ks, const float s) {
    return Reflector(colored(C_BLACK), ks, s, colored(C_BLACK), colored(C_BLACK));
}