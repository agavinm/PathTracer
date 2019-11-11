/******************************************************************************
 * @file    Scenes.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO3_SCENES_HPP
#define TRABAJO3_SCENES_HPP

#include <vector>
#include "Geometric.hpp"

/**
 * @return a list of objects in the scene
 */
std::vector<Object> getObjects(const std::string &scene);

/**
 * @return the camera
 * @param ratio width/height of image
 */
Camera getCamera(float ratio);

#endif //TRABAJO3_SCENES_HPP
