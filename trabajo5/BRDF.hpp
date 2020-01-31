//
// Created by abeln on 29/01/2020.
//

#ifndef TRABAJO5_BRDF_HPP
#define TRABAJO5_BRDF_HPP

#include "Scene.hpp"
#include "HCoord.hpp"
#include "Color.hpp"

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

HCoord getNewDirection(EVENT event, const HCoord &position, const HCoord &direction, const Object &object);

EVENT getRandomEvent(const Object &object, const HCoord &position);

Color getBRDF(EVENT event, const HCoord &in, const HCoord &out, const HCoord &position, const Object &object);

#endif //TRABAJO5_BRDF_HPP
