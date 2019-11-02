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
#include <cstring>

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
    cout << "[INFO] Storing image as ppm " << name << endl;

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

void storeBMP(const std::string &name, const Image &image) {
    // adapted from https://stackoverflow.com/a/2654860
    cout << "[INFO] Storing image as bmp " << name << endl;

#define WRITE(A, F) F.write(reinterpret_cast<const char *>(A), sizeof(A))

    // open output file stream
    ofstream f(name, ios::binary);
    if (!f.is_open()) {
        // can't open, exit
        cerr << "The file " << name << " can't be opened to write." << endl;
        exit(1);
    }

    // write header
    unsigned char header[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    int filesize = 54 + 3 * image.width * image.height;  //w is your image width, h is image height, both int
    header[2] = (unsigned char) (filesize);
    header[3] = (unsigned char) (filesize >> 8);
    header[4] = (unsigned char) (filesize >> 16);
    header[5] = (unsigned char) (filesize >> 24);
    WRITE(header, f);

    // write file info
    unsigned char info[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    info[4] = (unsigned char) (image.width);
    info[5] = (unsigned char) (image.width >> 8);
    info[6] = (unsigned char) (image.width >> 16);
    info[7] = (unsigned char) (image.width >> 24);
    info[8] = (unsigned char) (image.height);
    info[9] = (unsigned char) (image.height >> 8);
    info[10] = (unsigned char) (image.height >> 16);
    info[11] = (unsigned char) (image.height >> 24);
    WRITE(info, f);

    // write data
    unsigned char padding[(4 - (image.width * 3) % 4) % 4];
    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            int x = i;
            int y = (image.height - 1) - j;
            unsigned char r = (int) (image.pixels[x + y * image.width][0] / image.maxVal * 255);
            unsigned char g = (int) (image.pixels[x + y * image.width][1] / image.maxVal * 255);
            unsigned char b = (int) (image.pixels[x + y * image.width][2] / image.maxVal * 255);
            f << b << g << r;
        }
        WRITE(padding, f);
    }

}


// Tone mapping operators

Image equalizeAndClamp(const Image &image, float v) {
    cout << "[INFO] equalizeAndClamp -> ";
    return clampAndGammaCurve(image, v, 1);
}

Image clamping(const Image &image) {
    cout << "[INFO] clamping -> ";
    return clampAndGammaCurve(image, 1, 1);
}

Image equalization(const Image &image) {
    cout << "[INFO] equalization -> ";
    return clampAndGammaCurve(image, image.maxVal, 1);
}

Image gammaCurve(const Image &image, float gamma) {
    cout << "[INFO] gammaCurve -> ";
    return clampAndGammaCurve(image, image.maxVal, gamma);
}

Image clampAndGammaCurve(const Image &image, float v, float gamma) {
    cout << "[INFO] Clamping image with v=" << v << " and applying gamma curve with gamma=" << gamma << endl;
    Image imageOut;

    imageOut.width = image.width;
    imageOut.height = image.height;
    imageOut.maxVal = 1.0f;

    imageOut.pixels.resize(imageOut.width * imageOut.height); // Fix capacity
    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        for (int j = 0; j < 3; j++) {
            imageOut.pixels[i][j] = image.pixels[i][j] >= v ? 1.0f : pow(image.pixels[i][j] / v, 1 / gamma);
        }
    }

    return imageOut;
}

Image reinhard2002(const Image &image, float a) {
    cout << "[INFO] Reinhard2002-ing image with a=" << a << endl;
    int n = image.width * image.height;

    float Lw_average = 0;
    for (int i = 0; i < n; ++i)
        Lw_average += log(0.001f + (image.pixels[i][0] + image.pixels[i][1] + image.pixels[i][2]) / 3);
    Lw_average = exp(Lw_average / n);

    Image imageOut;

    imageOut.width = image.width;
    imageOut.height = image.height;
    float L = a * image.maxVal / Lw_average;
    imageOut.maxVal = L * (1 + L / image.maxVal * image.maxVal) / (1 + L);

    imageOut.pixels.resize(n); // Fix capacity

    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        for (int j = 0; j < 3; j++) {
            L = a * image.pixels[i][j] / Lw_average;
            imageOut.pixels[i][j] = L * (1 + L / image.maxVal * image.maxVal) / (1 + L);
        }
    }

    return imageOut;
}


