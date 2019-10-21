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

// Load and store operations

Image loadPPM(const std::string &name) {
    ifstream fin;
    fin.open(name);
    if (!fin.is_open()) {
        exit(1);
    }

    Image image;
    string line;

    getline(fin, line); // Format identification

    image.m = 1.0f;
    while (getline(fin, line) && line.front() == '#') { // Comments
        if (line.substr(0, 5) == "#MAX=") {
            image.m = stof(line.substr(5, line.size())); // Maximum
        }
    }

    int pos = line.find(' '); // Space position
    image.w = stoi(line.substr(0, pos + 1)); // Width
    image.h = stoi(line.substr(pos, line.size())); // Height

    getline(fin, line);
    image.c = stoi(line); // Color resolution

    float r, g, b;
    image.p.resize(image.w * image.h); // Fix capacity
    for (int i = 0; i < image.w * image.h; i++) { // Pixels
        fin >> r >> g >> b;
        image.p[i][0] = r * image.m / (float) image.c; // Red
        image.p[i][1] = g * image.m / (float) image.c; // Green
        image.p[i][2] = b * image.m / (float) image.c; // Blue
    }

    fin.close();
    return image;
}

void storePPM(const std::string &name, const Image &image) {
    ofstream fout;
    fout.open(name);
    if (!fout.is_open()) {
        exit(1);
    }

    fout << "P3" << endl; // Format identification

    fout << "#MAX=" << image.m << endl; // Maximum

    fout << image.w << " " << image.h << endl; // Width and height

    fout << image.c << endl; // Color resolution

    int r, g, b;
    for (int i = 0; i < image.w * image.h; i++) { // Pixels
        r = image.p[i][0] * image.c / image.m; // Red
        g = image.p[i][1] * image.c / image.m; // Green
        b = image.p[i][2] * image.c / image.m; // Blue
        fout << r << " " << g << " " << b << "     ";
    }

    fout.close();
}

// Tone mapping operators

Image equalizeAndClamp(const Image &image, float v, float m) {
    Image eImage;

    eImage.m = 1.0f;
    eImage.w = image.w;
    eImage.h = image.h;
    eImage.c = 65535;

    eImage.p.resize(eImage.w * eImage.h); // Fix capacity
    for (int i = 0; i < eImage.w * eImage.h; i++) { // Pixels
        for (int j = 0; j < 3; j++) {
            if (image.p[i][j] > v) {
                eImage.p[i][j] = v / m;
            }
            else {
                eImage.p[i][j] = image.p[i][j] / m;
            }
        }
    }

    return eImage;
}

Image equalizeAndClamp(const Image &image, float v) {
    return equalizeAndClamp(image, v, image.m);
}

Image clamping(const Image &image) {
    return equalizeAndClamp(image, 1.0f, 1.0f);
}

Image equalization(const Image &image) {
    return equalizeAndClamp(image, image.m, image.m);
}

