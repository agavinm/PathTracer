/******************************************************************************
 * @file    Render.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO3_RENDER_HPP
#define TRABAJO3_RENDER_HPP

#include "Image.hpp"
#include "Geometric.hpp"

/**
 * Renders "objects" seen by "camera" into a width * height image. All cores parallel processing.
 * @param width
 * @param height
 * @param ppp rays per pixel
 * @param objects scene
 * @param camera
 * @return the rendered image
 */
Image render(int width, int height, int ppp, const std::vector<Object> &objects, const Camera &camera);

/**
 * Renders "objects" seen by "camera" into a width * height image.
 * @param width
 * @param height
 * @param ppp rays per pixel
 * @param objects scene
 * @param camera
 * @param numThreads parallel processing
 * @return the rendered image
 */
Image render(int width, int height, int ppp, const std::vector<Object> &objects, const Camera &camera, int numThreads);

#endif //TRABAJO3_RENDER_HPP
