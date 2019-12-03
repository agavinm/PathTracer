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

Material Reflector(const Texture &kd, const float n, const Texture &ks, const float s, const Texture &kdPhong, const Texture &ksPhong) {
    return Material {
            .type = REFLECTOR,
            .property = {.reflectance = {.kd = kd, .n = n, .ks = ks, .kdPhong = kdPhong, .ksPhong = ksPhong, .s = s}}
    };
}

Material Diffuse(const Texture &k) {
    return Reflector(colored(C_BLACK), 0, colored(C_BLACK), 1, k, k);
}

Material Phong(const Texture &kdPhong, const Texture &ksPhong, const float s) {
    return Reflector(colored(C_BLACK), 0, colored(C_BLACK), s, kdPhong, ksPhong);
}

Material Delta(const Texture &kd, const float n, const Texture &ks, const float s) {
    return Reflector(kd, n, ks, s, colored(C_BLACK), colored(C_BLACK));
}

Material Refractor(const Texture &kd, const float n) {
    return Reflector(kd, n, colored(C_BLACK), 0, colored(C_BLACK), colored(C_BLACK));
}

Material Specular(const Texture &ks, const float s) {
    return Reflector(colored(C_BLACK), 0, ks, s, colored(C_BLACK), colored(C_BLACK));
}