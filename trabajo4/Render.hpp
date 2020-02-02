/******************************************************************************
 * @file    Render.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 5: Photon mapping
 ******************************************************************************/

#ifndef TRABAJO4_RENDER_HPP
#define TRABAJO4_RENDER_HPP

#include "Image.hpp"
#include "Scene.hpp"

/**
 * Renders "objects" seen by "camera" into a width * height image. All cores parallel processing.
 * @param width
 * @param height
 * @param ppp rays per pixel
 * @param scene
 * @return the rendered image
 */
void render(int width, int height, int ppp, const Scene &scene, const std::string& filename);

/**
 * Renders "objects" seen by "camera" into a width * height image.
 * @param width
 * @param height
 * @param ppp rays per pixel
 * @param scene
 * @param numThreads parallel processing
 * @return the rendered image
 */
Image render(int width, int height, int ppp, const Scene &scene, int numThreads);

#endif //TRABAJO4_RENDER_HPP
