/******************************************************************************
 * @file    Scene.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO4_SCENES_HPP
#define TRABAJO4_SCENES_HPP

#include <string>
#include <vector>
#include "Object.hpp"

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

// Scene

struct Scene {
    const Camera camera;
    const std::vector<Object> objects;
    const std::vector<LightPoint> lightPoints;
    const float refractiveIndex;
    const float gammaCorrection;
};

/**
 * @param scene name from predefined scenes ("default", "spiral", "XYZ") or triangular ply file
 * @return the scene
 */
Scene createScene(const std::string &scene, float ratio);

/**
 * Load the ply file into the objects array
 * @param filename file to load
 * @param objects array where the objects of the file will be appended
 */
void loadPly(const std::string &filename, std::vector<Object> &objects);

#endif //TRABAJO4_SCENE_HPP
