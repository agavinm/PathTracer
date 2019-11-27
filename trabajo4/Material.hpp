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
    EMITTER,
    REFLECTOR
};

struct Reflectance {
    const Texture kd; // Diffuse (delta BTDF)
    const Texture ks; // Specular (delta BRDF)
    const float kp; // Phong (how Phong is it) (Phong BRDF)
};

typedef struct {
    const MATERIAL_TYPE type;
    const union {
        const Texture texture;
        const Reflectance reflectance;
    } property;
} Material;

/**
 * Creates an emmiter material
 * @return the emmiter as material
 */
Material Emitter(const Texture &texture);

/**
 * Creates a reflector material
 * @return the reflector as material
 */
Material Reflector(const Texture &kd, const Texture &ks, float kp);

#endif //TRABAJO3_MATERIAL_HPP
