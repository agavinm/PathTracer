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
    DELTA, // delta BRDF/BTDF
    PHONG // Phong BRDF
};

struct Reflectance {
    const Texture kd; // Diffuse
    const Texture ks; // Specular
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
 * Creates a Delta material
 * @return the Delta material
 */
Material Delta(const Texture &kd, const Texture &ks);

/**
 * Creates a perfect diffuse Delta material
 * @return the perfect diffuse Delta material
 */
Material Diffuse(const Texture &kd);

/**
 * Creates a Phong material
 * @return the Phong material
 */
Material Phong(const Texture &kd, const Texture &ks);

#endif //TRABAJO3_MATERIAL_HPP
