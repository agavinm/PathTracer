/******************************************************************************
 * @file    pathtracer.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include <iostream>
#include "Render.hpp"

using namespace std;

bool endsWith(const string &original, const string &suffix) {
    return original.compare(original.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void printUsage(const string &name) {
    cout << "Usage: " << name << " <ppp> <width> <height> <file name> <scene>" << endl
            << "* <file name> needs to be one of" << endl
            << "       - .ppm" << endl
            << "       - .bmp" << endl
            << "OPTIONAL* <scene> can be a triangular .ply file or a predefined scene:" << endl
            << "       default" << endl
            << "       specular" << endl
            << "       refraction" << endl;
}

int main(int argc, char *argv[]) {
    // check enough arguments
    if (argc < 5 || argc > 6) {
        printUsage(argv[0]);
        return 0;
    }

    // rays
    int ppp = stoi(argv[1]);

    // Image
    int width = stoi(argv[2]);
    int height = stoi(argv[3]);

    // Scene
    string sceneName = "default";
    if (argc == 6) {
        sceneName = argv[5];
    }
    Scene scene = createScene(sceneName, (float) width / (float) height);

    if (endsWith(argv[4], ".ppm")) {
        storePPM(string(argv[4]), render(width, height, ppp, scene), 255);
    }
    else if (endsWith(argv[4], ".bmp")) {
        storeBMP(string(argv[4]), render(width, height, ppp, scene));
    }
    else {
        cerr << "Unknown output file extension" << endl;
        return 1;
    }

    return 0;
}
