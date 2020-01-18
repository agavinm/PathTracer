/******************************************************************************
 * @file    Image.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Diciembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Image initImage(int width, int height) {
    return {
        .width = width,
        .height = height,
        .pixels = vector<Color>(width * height)
    };
}

void setPixel(Image &image, int i, int j, Color pixel) {
    image.pixels[i + j * image.width] = pixel;
}

float getMaxVal(const Image &image){
    float maxVal = 0;
    for (auto pixel : image.pixels)
        maxVal = max(pixel.max(), maxVal);
    return maxVal;
}


///////////////////////////////////////////////////////

static const string MAX_SPECIFICATION = "#MAX=";
static const string HEADER = "P3";
static const char COMMENT = '#';
static const char SPACE = ' ';

// Load and store operations

void storePPM(const std::string &name, const Image &image, int resolution) {
    cout << "[INFO] Storing image as ppm " << name << endl;

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
    float maxVal = getMaxVal(image);
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
        r = (int) (image.pixels[i].r / maxVal) * resolution; // Red
        g = (int) (image.pixels[i].g / maxVal) * resolution; // Green
        b = (int) (image.pixels[i].b / maxVal) * resolution; // Blue
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
    float maxVal = getMaxVal(image);
    unsigned char padding[(4 - (image.width * 3) % 4) % 4];
    for (int j = 0; j < image.height; j++) {
        for (int i = 0; i < image.width; i++) {
            int x = i;
            int y = (image.height - 1) - j;
            unsigned char r = (int) (image.pixels[x + y * image.width].r / maxVal * 255);
            unsigned char g = (int) (image.pixels[x + y * image.width].g / maxVal * 255);
            unsigned char b = (int) (image.pixels[x + y * image.width].b / maxVal * 255);
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
    return clampAndGammaCurve(image, getMaxVal(image), 1);
}

Image gammaCurve(const Image &image, float gamma) {
    cout << "[INFO] gammaCurve -> ";
    return clampAndGammaCurve(image, getMaxVal(image), gamma);
}

Image clampAndGammaCurve(const Image &image, float v, float gamma) {
    cout << "[INFO] Clamping image with v=" << v << " and applying gamma curve with gamma=" << gamma << endl;
    Image imageOut = {
            .width = image.width,
            .height = image.height,
            .pixels = vector<Color>(image.width * image.height)
    };

    imageOut.pixels.resize(imageOut.width * imageOut.height); // Fix capacity
    for (int i = 0; i < imageOut.width * imageOut.height; i++) {
        // foreach pixel, compute
        imageOut.pixels[i].r = image.pixels[i].r >= v ? 1.0f : pow(image.pixels[i].r / v, 1.0f / gamma);
        imageOut.pixels[i].g = image.pixels[i].g >= v ? 1.0f : pow(image.pixels[i].g / v, 1.0f / gamma);
        imageOut.pixels[i].b = image.pixels[i].b >= v ? 1.0f : pow(image.pixels[i].b / v, 1.0f / gamma);
    }

    return imageOut;
}