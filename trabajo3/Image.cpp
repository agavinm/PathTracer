/******************************************************************************
 * @file    Image.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include "Image.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

Image initImage(int width, int height) {
    return {
            .width=width,
            .height=height,
            .pixels=vector<Color>(width * height),
    };
}

void setPixel(Image &image, int i, int j, Color pixel) {
    image.pixels[i + j * image.width] = pixel;
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

void storeBMP(const std::string &name, const Image &image) {
    // adapted from https://stackoverflow.com/a/2654860
    cout << "[INFO] Storing image as bmp " << name << endl;

    float maxVal = 0;
    for (auto pixel : image.pixels)
        maxVal = max(pixel.max(), maxVal);

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
            unsigned char r = (int) (image.pixels[x + y * image.width].r / maxVal * 255);
            unsigned char g = (int) (image.pixels[x + y * image.width].g / maxVal * 255);
            unsigned char b = (int) (image.pixels[x + y * image.width].b / maxVal * 255);
            f << b << g << r;
        }
        WRITE(padding, f);
    }
}