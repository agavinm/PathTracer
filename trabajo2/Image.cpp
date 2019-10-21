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

using namespace std;

static const string MAX_SPECIFICATION = "#MAX=";
static const string HEADER = "P3";
static const char COMMENT = '#';
static const char SPACE = ' ';

// Load and store operations


Image loadPPM(const string &name) {

    // open input file stream
    ifstream file(name);
    if (!file.is_open()) {
        // can't open, exit
        cout << "The file " << name << " can't be opened to read." << endl;
        exit(1);
    }

    // vars
    Image image;
    string line;

    // check header (first line)
    getline(file, line); // Format identification
    if (line != HEADER) {
        // no header, but let's continue just in case
        cout << "[WARNING] The image don't start with '" << HEADER << "' but '" << line << "' instead. " << endl;
    }

    // get max value from comments
    // TODO allow comments in other positions too
    float maxVal = 1.0f;
    while(file.peek() == COMMENT){
        getline(file, line);
        if (line.compare(0, MAX_SPECIFICATION.size(), MAX_SPECIFICATION)==0) {
            maxVal = stof(line.substr(MAX_SPECIFICATION.size(), line.size()));
        }
    }

    // get width/height
    file >> image.width >> image.height;

    // get color resolution
    float colorRes;
    file >> colorRes;

    // get rgb pixels
    float r, g, b;
    image.pixels.resize(image.width * image.height); // Fix capacity
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        file >> r >> g >> b;
        image.pixels[i][0] = r * maxVal / (float) colorRes; // Red
        image.pixels[i][1] = g * maxVal / (float) colorRes; // Green
        image.pixels[i][2] = b * maxVal / (float) colorRes; // Blue
    }

    return image;
}

// assumes the max value is 1.0f
void storePPM(const std::string &name, const Image &image, int resolution) {

    // open output file stream
    ofstream fout(name);
    if (!fout.is_open()) {
        // can't open, exit
        cout << "The file " << name << " can't be opened to write." << endl;
        exit(1);
    }

    // write header
    fout << HEADER << endl;

    // write width/height
    fout << image.width << SPACE << image.height << endl;

    // write color resolution
    fout << resolution << endl;

    // write rgb pixels
    int r, g, b;
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        r = image.pixels[i][0] * resolution; // Red
        g = image.pixels[i][1] * resolution; // Green
        b = image.pixels[i][2] * resolution; // Blue
        fout << r << SPACE << g << SPACE << b << string(5, SPACE);
    }
}

// Tone mapping operators

Image equalizeAndClamp(const Image &image, float v) {
    Image imageOut;

    imageOut.width = image.width;
    imageOut.height = image.height;

    imageOut.pixels.resize(imageOut.width * imageOut.height); // Fix capacity
    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        for (int j = 0; j < 3; j++) {
            if (image.pixels[i][j] >= v) {
                imageOut.pixels[i][j] = 1.0f;
            } else {
                imageOut.pixels[i][j] = image.pixels[i][j] / v;
            }
        }
    }

    return imageOut;
}

Image clamping(const Image &image) {
    return equalizeAndClamp(image, 1.0f);
}

Image equalization(const Image &image) {
    // find max value in image
    float max = 0;
    for (int i = 0; i < image.width * image.height; i++) {
        for (int j = 0; j < 3; j++) {
            if (image.pixels[i][j] > max) {
                max = image.pixels[i][j];
            }
        }
    }

    // equalize
    return equalizeAndClamp(image, max);
}

