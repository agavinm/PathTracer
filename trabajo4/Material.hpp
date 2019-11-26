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
    REFLECTER,
    BRDF
};

struct Reflection {
    const Texture kd; // Diffuse
    const Texture ks; // Specular
};

typedef struct {
    const MATERIAL_TYPE type;
    const union {
        const Texture texture;
        const Reflection k;
    } property;
} Material;

/**
 * Creates an emmiter material
 * @return the emmiter as material
 */
Material Emitter(const Texture &texture);

/**
 * Creates a reflecter material
 * @return the reflecter as material
 */
Material Reflecter(const Texture &kd, const Texture &ks);

/**
 * Creates a phong brdf material
 * @return the phong brdf as material
 */
Material Brdf(const Texture &kd, const Texture &ks);

#endif //TRABAJO3_MATERIAL_HPP
