/******************************************************************************
 * @file    Material.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO3_MATERIAL_HPP
#define TRABAJO3_MATERIAL_HPP

#include "Image.hpp"
#include "Texture.hpp"
#include "Geometric.hpp"

// Materials

enum MATERIAL_TYPE {
    EMITTER,
    TEXTURE
};

typedef struct {
    const MATERIAL_TYPE type;
    const union {
        const COLOR emitter;
        const Texture texture;
    } data;
} MATERIAL;

/**
 * Creates an emitter material
 * @return the emitter as material
 */
MATERIAL Emitter(const COLOR &color);

/**
 * Creates a textured material
 * @return the texturer as material
 */
MATERIAL Texturer(const Texture &texture);

// Objects

struct Object {
    const GEOMETRY geometry;
    const MATERIAL material;
};

/**
 * Calculates the intersection between an object and a ray
 * @param origin origin of ray
 * @param dir direction of ray
 * @param object the object to check colision
 * @return the distance between the origin and the intersection, or INFINITY if not collided
 */
float intersect(const HCoord &origin, const HCoord &dir, const Object &object);

#endif //TRABAJO3_MATERIAL_HPP
