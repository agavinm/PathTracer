//******************************************************************************
// File:   Image.hpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Octubre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 2
//         Imaging
//******************************************************************************

#ifndef TRABAJO2_IMAGE_HPP
#define TRABAJO2_IMAGE_HPP

#include <string>
#include <vector>
#include <array>

/*
 * maxVal = maximum value of the pixels
 * width = Width
 * height = Height
 * pixels = width*height RGB pixel array (stored as floating point precision)
 */
struct Image {
    int width, height;
    std::vector<std::array<float, 3>> pixels; //TODO: allow 2d vector access
};

Image initImage(int width, int height);

void setPixel(Image &image, int i, int j, float r, float g, float b);

// Load and store operations

void storePPM(const std::string &name, const Image &image, int resolution);

#endif //TRABAJO2_IMAGE_HPP
