//******************************************************************************
// File:   Image.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Octubre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 2
//         Imaging
//******************************************************************************

#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

float COLOR::max() const {
    return std::max(std::max(this->r, this->g), this->b);
}

COLOR COLOR::operator+(const COLOR &right) const {
    return {
            this->r + right.r,
            this->g + right.g,
            this->b + right.b
    };
}

COLOR COLOR::operator/(float s) const {
    return {
            this->r / s,
            this->g / s,
            this->b / s
    };
}

Image initImage(int width, int height) {
    return {
            .width=width,
            .height=height,
            .pixels=vector<COLOR>(width * height),
    };
}

void setPixel(Image &image, int i, int j, COLOR pixel) {
    image.pixels[i + (image.height - 1 - j) * image.width] = pixel;
}


///////////////////////////////////////////////////////

static const string MAX_SPECIFICATION = "#MAX=";
static const string HEADER = "P3";
static const char COMMENT = '#';
static const char SPACE = ' ';

// Load and store operations

void storePPM(const std::string &name, const Image &image, int resolution) {
    cout << "[INFO] Storing image as ppm " << name << endl;

    float maxVal = 0;
    for (auto pixel : image.pixels)
        maxVal = max(pixel.max(), maxVal);

    // open output file stream
    ofstream fout(name);
    if (!fout.is_open()) {
        // can't open, exit
        cerr << "The file " << name << " can't be opened to write." << endl;
        exit(10);
    }

    // write header
    fout << HEADER << endl;

    // write maxval (if not 1)
    if (maxVal != 1.0f) {
        fout << MAX_SPECIFICATION << maxVal << endl;
    }

    // write width/height
    fout << image.width << SPACE << image.height << endl;

    // write color resolution
    fout << resolution << endl;

    // write rgb pixels
    int r, g, b;
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        r = image.pixels[i].r * resolution / maxVal; // Red
        g = image.pixels[i].g * resolution / maxVal; // Green
        b = image.pixels[i].b * resolution / maxVal; // Blue
        fout << r << SPACE << g << SPACE << b << string(5, SPACE);
    }
}
