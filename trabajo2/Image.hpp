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
 * maxVal = pixels maximum of the image (stored as floating point precision; 1 is LDR; >1 is HDR)
 * width = Width
 * height = Height
 * colorRes = Color resolution
 * pixels = width*height RGB pixel array (stored as floating point precision)
 */
struct Image {
    float maxVal;
    int width, height, colorRes; // TODO: colorRes is not part of the image, should be removed
    std::vector<std::array<float, 3>> pixels; //TODO: to a 2d vector (will be useful later)
};

// Load and store operations

Image loadPPM(const std::string &name);

void storePPM(const std::string &name, const Image &image);

// Tone mapping operators

Image clamping(const Image &image);

Image equalization(const Image &image);

Image equalizeAndClamp(const Image &image, float v);

Image gammaCurve(const Image &image);

Image clampAndGammaCurve(const Image &image, float v);

#endif //TRABAJO2_IMAGE_HPP
