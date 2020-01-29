//
// Created by abeln on 29/01/2020.
//

#ifndef TRABAJO5_BRDF_HPP
#define TRABAJO5_BRDF_HPP


pair<Color, HCoord> reflection(const HCoord &position, const HCoord &direction, const HCoord &n, const Object &intersection);

pair<Color, HCoord> refraction(const Scene &scene, const HCoord &origin, const HCoord &position, const HCoord &direction, HCoord n, const Object &intersection);

pair<Color, HCoord> phong(const Scene &scene, const HCoord &position, const HCoord &direction, const HCoord &n,
                          const Object &intersection);

enum EVENT {
    REFRACTION,
    REFLECTION,
    PHONG,
    DEAD
};

EVENT getRandomEvent(const Object &object, const HCoord & position);

Color getBRDF(const HCoord &in, const HCoord &out, const HCoord &position, const Object &object);
#endif //TRABAJO5_BRDF_HPP
