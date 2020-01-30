//
// Created by abeln on 29/01/2020.
//

#ifndef TRABAJO5_BRDF_HPP
#define TRABAJO5_BRDF_HPP

#include "Scene.hpp"
#include "HCoord.hpp"
#include "Color.hpp"

std::pair<Color, HCoord> reflection(const HCoord &position, const HCoord &direction, const HCoord &n, const Object &intersection);

std::pair<Color, HCoord> refraction(const Scene &scene, const HCoord &origin, const HCoord &position, const HCoord &direction, HCoord n, const Object &intersection);

std::pair<Color, HCoord> phong(const Scene &scene, const HCoord &position, const HCoord &direction, const HCoord &n,
                          const Object &intersection);

/**
 * Represents one of the possible events of the render equation
 */
enum EVENT {
    REFRACTION,
    REFLECTION,
    PHONG,
    DEAD
};

/**
 * The 'Russian roulette'
 * @param object of intersection
 * @param position of intersection
 * @return a random valid event
 */
EVENT getRandomEvent(const Object &object, const HCoord &position);

/**
 * The Equation
 * @param event which event to compute
 * @param in input vector, towards the bounce
 * @param out output vector, away from the bounce
 * @param position where the bounce ocurred
 * @param object involved in the bounce
 * @return the factor (color) of the bounce
 */
Color getBRDF(EVENT event, const HCoord &in, const HCoord &out, const HCoord &position, const Object &object);

#endif //TRABAJO5_BRDF_HPP
