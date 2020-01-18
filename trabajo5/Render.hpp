/******************************************************************************
 * @file    Render.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
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
 * @param fotons number of fotons to launch (global, distributed randomly between lightpoints)
 * @param scene
 * @return the rendered image
 */
Image render(int width, int height, int ppp, int fotons, const Scene &scene);

/**
 * Renders "objects" seen by "camera" into a width * height image.
 * @param width
 * @param height
 * @param ppp rays per pixel
 * @param fotons number of fotons to launch (global, distributed randomly between lightpoints)
 * @param scene
 * @param numThreads parallel processing
 * @return the rendered image
 */
Image render(int width, int height, int ppp, int fotons, const Scene &scene, int numThreads);

#endif //TRABAJO4_RENDER_HPP
