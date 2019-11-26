/******************************************************************************
 * @file    Light.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO4_LIGHT_HPP
#define TRABAJO4_LIGHT_HPP

#include "Color.hpp"

struct LightPoint {
    const Color color;
    const HCoord point;
};

#endif //TRABAJO4_LIGHT_HPP
