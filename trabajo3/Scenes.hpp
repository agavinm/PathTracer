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
#include "Material.hpp"

// Camera

struct Camera {
    const HCoord origin; // center of camera (UCS)
    const HCoord front; // front vector of camera (UCS)
    const HCoord left; // left vector of camera (UCS)
    const HCoord up; // up vector of camera (UCS)
};

/**
 * Creates a camera
 * @param origin point (UCS)
 * @param front direction from origin to center of image (UCS)
 * @param up direction from center of image to top side of image
 * @param ratio width/height of image
 * @return the camera
 */
Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio);

/**
 * Computes the direction of a ray from a camera
 * @param camera origin of ray
 * @param i horizontal position in image (0=left -> 1=right)
 * @param j vertical position in image (0=up -> 1=bottom)
 * @return the ray as vector
 */
HCoord getRay(const Camera &camera, float i, float j);

/**
 * @param scene name from predefined scenes ("test", "spiral", "XYZ") or triangular ply file
 * @return a list of objects in the scene
 */
std::vector<Object> getObjects(const std::string &scene);

/**
 * @return the camera
 * @param ratio width/height of image
 */
Camera getCamera(float ratio);

#endif //TRABAJO3_SCENES_HPP
