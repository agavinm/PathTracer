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