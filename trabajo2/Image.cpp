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

static const string MAX_SPECIFICATION = "#MAX=";
static const string HEADER = "P3";
static const char COMMENT = '#';
static const char SPACE = ' ';

// Load and store operations

Image loadPPM(const string &name) {
    cout << "[INFO] Loading image " << name << endl;

    // open input file stream
    ifstream file(name);
    if (!file.is_open()) {
        // can't open, exit
        cerr << "The file " << name << " can't be opened to read." << endl;
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
    while (file.peek() == COMMENT) {
        getline(file, line);
        if (line.compare(0, MAX_SPECIFICATION.size(), MAX_SPECIFICATION) == 0) {
            image.maxVal = stof(line.substr(MAX_SPECIFICATION.size(), line.size()));
        }
    }

    // get width/height
    file >> image.width >> image.height;

    // get color resolution
    float colorRes;
    file >> colorRes;

    // get rgb pixels
    float r, g, b;
    bool maxFound = false;
    image.pixels.resize(image.width * image.height); // Fix capacity
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        file >> r >> g >> b;
        image.pixels[i][0] = r * image.maxVal / (float) colorRes; // Red
        image.pixels[i][1] = g * image.maxVal / (float) colorRes; // Green
        image.pixels[i][2] = b * image.maxVal / (float) colorRes; // Blue

        if (r == colorRes || g == colorRes || b == colorRes) maxFound = true;
    }

    if (!maxFound) {
        cerr << "[WARNING] Maximum value (" << colorRes << ") not found in file";
    }

    return image;
}

void storePPM(const std::string &name, const Image &image, int resolution) {
    cout << "[INFO] Storing image " << name << endl;

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
    if (image.maxVal != 1.0f) {
        fout << MAX_SPECIFICATION << image.maxVal << endl;
    }

    // write width/height
    fout << image.width << SPACE << image.height << endl;

    // write color resolution
    fout << resolution << endl;

    // write rgb pixels
    int r, g, b;
    for (int i = 0; i < image.width * image.height; i++) {
        // foreach pixel, compute
        r = image.pixels[i][0] * resolution / image.maxVal; // Red
        g = image.pixels[i][1] * resolution / image.maxVal; // Green
        b = image.pixels[i][2] * resolution / image.maxVal; // Blue
        fout << r << SPACE << g << SPACE << b << string(5, SPACE);
    }
}


// Tone mapping operators

Image equalizeAndClamp(const Image &image, float v) {
    cout << "(equalizeAndClamp) ";
    return clampAndGammaCurve(image, v, 1);
}

Image clamping(const Image &image) {
    cout << "(clamping) ";
    return clampAndGammaCurve(image, 1, 1);
}

Image equalization(const Image &image) {
    cout << "(equalization) ";
    return clampAndGammaCurve(image, image.maxVal, 1);
}

Image gammaCurve(const Image &image, float gamma) {
    cout << "(gammaCurve) ";
    return clampAndGammaCurve(image, image.maxVal, gamma);
}

Image clampAndGammaCurve(const Image &image, float v, float gamma) {
    cout << "[INFO] Clamping image with v=" << v << " and applying gamma curve with gamma=" << gamma << endl;
    Image imageOut;

    imageOut.width = image.width;
    imageOut.height = image.height;
    imageOut.maxVal = image.maxVal >= v ? 1.0f : image.maxVal / v;

    imageOut.pixels.resize(imageOut.width * imageOut.height); // Fix capacity
    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        for (int j = 0; j < 3; j++) {
            imageOut.pixels[i][j] = image.pixels[i][j] >= v ? 1.0f : pow(image.pixels[i][j] / v, 1 / gamma);
        }
    }

    return imageOut;
}

