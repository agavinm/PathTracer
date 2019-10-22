//******************************************************************************
// File:   main.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Octubre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 2
//******************************************************************************

#include <iostream>
#include "Image.hpp"

using namespace std;

bool endsWith(const string &original, const string &suffix) {
    return original.compare(original.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void printUsage(const string &name) {
    cout << "Usage: " << name << " <file input> <file output> {operator} [operator parameters...]" << endl
         << "* <file input> needs to be a .ppm file" << endl
         << "* <file output> needs to be one of" << endl
         << "       - .ppm" << endl
         << "       - .bmp" << endl
         << "* <operator> needs to be one of: " << endl
         << "       - clamping" << endl
         << "       - equalization " << endl
         << "       - equalizeAndClamp <v>" << endl
         << "       - gammaCurve <gamma>" << endl
         << "       - clampAndGammaCurve <v> <gamma>" << endl;
}

int main(int argc, char *argv[]) {

    // for convenience
    vector<string> args(argv, argv + argc);

    // check enough arguments
    if (argc < 3) {
        printUsage(args[0]);
        return 0;
    }

    // load file
    if (!endsWith(args[1], ".ppm")) {
        cerr << "Input file isn't a .ppm" << endl;
        printUsage(args[0]);
        return 1;
    }
    Image img_hdr = loadPPM(args[1]);

    // check and apply operator
    Image img_ldr;
    if (args[3] == "clamping") {
        img_ldr = clamping(img_hdr);

    } else if (args[3] == "equalization") {
        img_ldr = equalization(img_hdr);

    } else if (args[3] == "equalizeAndClamp") {
        if (argc < 4) {
            cerr << "Invalid number of parameters to " << args[3] << endl;
            printUsage(args[0]);
            return 1;
        }
        img_ldr = equalizeAndClamp(img_hdr, stof(args[4]));

    } else if (args[3] == "gammaCurve") {
        if (argc < 4) {
            cerr << "Invalid number of parameters to " << args[3] << endl;
            printUsage(args[0]);
            return 1;
        }
        img_ldr = gammaCurve(img_hdr, stof(args[4]));

    } else if (args[3] == "clampAndGammaCurve") {
        if (argc < 5) {
            cerr << "Invalid number of parameters to " << args[3] << endl;
            printUsage(args[0]);
            return 1;
        }
        img_ldr = clampAndGammaCurve(img_hdr, stof(args[4]), stof(args[5]));

    } else {
        cerr << "Unknown operator " << args[3] << endl;
        printUsage(args[0]);
        return 1;
    }

    // save file
    if (endsWith(args[2], ".ppm")) {
        storePPM(args[2], img_ldr, 255);

    } else if (endsWith(args[2], ".bmp")) {
        storeBMP(args[2], img_ldr);

    } else {
        cerr << "Unknown output file extension" << endl;
    }

    return 0;
}