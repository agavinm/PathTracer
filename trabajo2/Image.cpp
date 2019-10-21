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
    image.maxVal = 1.0f;
    while(file.peek() == COMMENT){
        getline(file, line);
        if (line.compare(0, MAX_SPECIFICATION.size(), MAX_SPECIFICATION)==0) {
            image.maxVal = stof(line.substr(MAX_SPECIFICATION.size(), line.size()));
        }
    }

    // get width/height
    file >> image.width >> image.height;

    // get color resolution
    file >> image.colorRes;

    // get rgb pixels
    float r, g, b;
    image.pixels.resize(image.width * image.height); // Fix capacity
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        file >> r >> g >> b;
        image.pixels[i][0] = r * image.maxVal / (float) image.colorRes; // Red
        image.pixels[i][1] = g * image.maxVal / (float) image.colorRes; // Green
        image.pixels[i][2] = b * image.maxVal / (float) image.colorRes; // Blue
    }

    return image;
}

void storePPM(const std::string &name, const Image &image) {

    // open output file stream
    ofstream fout(name);
    if (!fout.is_open()) {
        // can't open, exit
        cout << "The file " << name << " can't be opened to write." << endl;
        exit(1);
    }

    // write header
    fout << HEADER << endl;

    // write maximum
    fout << MAX_SPECIFICATION << image.maxVal << endl;

    // write width/height
    fout << image.width << SPACE << image.height << endl;

    // write color resolution
    fout << image.colorRes << endl;

    // write rgb pixels
    int r, g, b;
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        r = image.pixels[i][0] * image.colorRes / image.maxVal; // Red
        g = image.pixels[i][1] * image.colorRes / image.maxVal; // Green
        b = image.pixels[i][2] * image.colorRes / image.maxVal; // Blue
        fout << r << SPACE << g << SPACE << b << string(5, SPACE);
    }
}

// Tone mapping operators

Image equalizeAndClamp(const Image &image, float v, float maxVal) {
    Image imageOut;

    imageOut.maxVal = 1.0f;
    imageOut.width = image.width;
    imageOut.height = image.height;
    imageOut.colorRes = 255; // TODO: this parameter should be in storePPM

    imageOut.pixels.resize(imageOut.width * imageOut.height); // Fix capacity
    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        for (int j = 0; j < 3; j++) {
            if (image.pixels[i][j] > v) {
                imageOut.pixels[i][j] = v / maxVal;
            } else {
                imageOut.pixels[i][j] = image.pixels[i][j] / maxVal;
            }
        }
    }

    return imageOut;
}

Image equalizeAndClamp(const Image &image, float v) {
    return equalizeAndClamp(image, v, image.maxVal);
}

Image clamping(const Image &image) {
    return equalizeAndClamp(image, 1.0f, 1.0f);
}

Image equalization(const Image &image) {
    return equalizeAndClamp(image, image.maxVal, image.maxVal);
}

