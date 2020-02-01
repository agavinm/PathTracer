/******************************************************************************
 * @file    BRDF.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#ifndef TRABAJO4_BRDF_HPP
#define TRABAJO4_BRDF_HPP

#include "Scene.hpp"
#include "HCoord.hpp"
#include "Color.hpp"
#include <stack>

/**
 * Represents one of the possible events of the render equation
 */
enum EVENT {
    REFRACTION,
    REFLECTION,
    PHONG_DIFFUSE,
    PHONG_SPECULAR,
    DEAD
};

HCoord getNewDirection(EVENT event, const HCoord &position, const HCoord &direction, const Object &object,
        std::stack<const Object*> &refractionStack, float sceneRefractiveIndex);

EVENT getRandomEvent(const Object &object, const HCoord &position);

Color getBRDF(EVENT event, const HCoord &in, const HCoord &out, const HCoord &position, const Object &object);

#endif //TRABAJO4_BRDF_HPP
