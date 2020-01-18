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

Material Phong(const Texture &kdPhong, const Texture &ksPhong, const float s) {
    return Reflector(colored(C_BLACK), colored(C_BLACK), s, kdPhong, ksPhong);
}

Material Diffuse(const Texture &k) {
    return Phong(k, k, 1);
}

Material Delta(const Texture &kd, const Texture &ks) {
    return Reflector(kd, ks, 1, colored(C_BLACK), colored(C_BLACK));
}

Material Refractor(const Texture &kd) {
    return Delta(kd, colored(C_BLACK));
}

Material Specular(const Texture &ks) {
    return Delta(colored(C_BLACK), ks);
}