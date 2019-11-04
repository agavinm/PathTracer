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

Image initImage(int width, int height) {
    Image image;
    image.width = width;
    image.height = height;
    image.pixels.resize(width * height);
    return image;
}

void setPixel(Image &image, int i, int j, float r, float g, float b) {
    image.pixels[i + (image.height - 1 - j) * image.width][0] = r;
    image.pixels[i + (image.height - 1 - j) * image.width][1] = g;
    image.pixels[i + (image.height - 1 - j) * image.width][2] = b;
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
    for (int i = 0; i < image.pixels.size(); ++i)
        for (int j = 0; j < 3; ++j)
            maxVal = max(image.pixels[i][j], maxVal);

    // open output file stream
    ofstream fout(name);
    if (!fout.is_open()) {
        // can't open, exit
        cerr << "The file " << name << " can't be opened to write." << endl;
        exit(1);
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
        r = image.pixels[i][0] * resolution / maxVal; // Red
        g = image.pixels[i][1] * resolution / maxVal; // Green
        b = image.pixels[i][2] * resolution / maxVal; // Blue
        fout << r << SPACE << g << SPACE << b << string(5, SPACE);
    }
}

