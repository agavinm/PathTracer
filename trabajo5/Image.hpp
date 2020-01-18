/******************************************************************************
 * @file    Image.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO2_IMAGE_HPP
#define TRABAJO2_IMAGE_HPP

#include <string>
#include <vector>
#include <array>
#include "Color.hpp"

/**
 * maxVal = maximum value of the pixels
 * width = Width
 * height = Height
 * pixels = width*height RGB pixel array (stored as floating point precision)
 */
struct Image {
    const int width, height;
    std::vector<Color> pixels;
};

/**
 * Initializes an image
 */
Image initImage(int width, int height);

/**
 * Sets a pixel of an image
 * @param image to modify
 * @param i horizontal coordinate, [0,width-1]
 * @param j vertical coordinate [0,height-1]
 * @param pixel RGB color
 */
void setPixel(Image &image, int i, int j, Color pixel);

// Load and store operations

/**
 * Saves the image as a ppm file
 * @param name name of the file with extension
 * @param image what to store
 * @param resolution max value to store, 255 for standard ppm
 */
void storePPM(const std::string &name, const Image &image, int resolution);

void storeBMP(const std::string &name, const Image &image);

// Tone mapping operators

Image clamping(const Image &image);

Image equalization(const Image &image);

Image equalizeAndClamp(const Image &image, float v);

Image gammaCurve(const Image &image, float gamma);

Image clampAndGammaCurve(const Image &image, float v, float gamma);

#endif //TRABAJO2_IMAGE_HPP
