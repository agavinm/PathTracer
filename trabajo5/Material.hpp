/******************************************************************************
 * @file    Material.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
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
    const Texture kd; // Refraction (delta BTDF)
    const Texture ks; // Specular (delta BRDF)
    const Texture kdPhong; //Phong BRDF
    const Texture ksPhong; // Phong BRDF
    const float s; // Shininess
};

/**
 * EMMITER
 * Light area
 *
 * REFLECTOR
 * Reflector material
 * Perfect Diffuse material: kd = 0 AND ks = 0 AND ksPhong = kdPhong
 * Perfect Phong material: kd = 0 AND ks = 0
 * Perfect Delta material: kdPhong = 0 AND ksPhong = 0
 * Perfect Refractor material: ks = 0 AND kdPhong = 0 AND ksPhong = 0
 * Perfect Specular material: kd = 0 AND kdPhong = 0 AND ksPhong = 0
 */
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
 * Creates a Reflector material
 * @return the Reflector material
 */
Material Reflector(const Texture &kd, const Texture &ks, float s, const Texture &kdPhong, const Texture &ksPhong);

/**
 * Creates a perfect Phong material
 * @return the perfect Phong material
 */
Material Phong(const Texture &kdPhong, const Texture &ksPhong, float s);

/**
 * Creates a perfect diffuse Delta material
 * @return the perfect diffuse Delta material
 */
Material Diffuse(const Texture &k);

/**
 * Creates a perfect Delta material
 * @return the perfect Delta material
 */
Material Delta(const Texture &kd, const Texture &ks);

/**
 * Creates a perfect Refractor material
 * @return the perfect Refractor material
 */
Material Refractor(const Texture &kd);

/**
 * Creates a perfect Specular material
 * @return the perfect Specular material
 */
Material Specular(const Texture &ks);

#endif //TRABAJO3_MATERIAL_HPP
