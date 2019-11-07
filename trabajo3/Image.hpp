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

struct COLOR {
    float r;
    float g;
    float b;

    float max() const;

    COLOR operator+(const COLOR &right) const;

    COLOR operator/(float s) const;

};

/**
 * maxVal = maximum value of the pixels
 * width = Width
 * height = Height
 * pixels = width*height RGB pixel array (stored as floating point precision)
 */
struct Image {
    const int width, height;
    std::vector<COLOR> pixels;
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
 * @param r red
 * @param g green
 * @param b blue
 */
void setPixel(Image &image, int i, int j, COLOR pixel);

// Load and store operations

/**
 * Saves the image as a ppm file
 * @param name name of the file with extension
 * @param image what to store
 * @param resolution max value to store, 255 for standard ppm
 */
void storePPM(const std::string &name, const Image &image, int resolution);

#endif //TRABAJO2_IMAGE_HPP
