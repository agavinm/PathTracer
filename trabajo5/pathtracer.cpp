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
    cout << "Usage: " << name << " <ppp> <fotons> <width> <height> <file name> <scene>" << endl
            << "* <file name> needs to be one of" << endl
            << "       - .ppm" << endl
            << "       - .bmp" << endl
            << "OPTIONAL* <scene> can be a triangular .ply file or a predefined scene." << endl;
    printScenes();
}

int main(int argc, char *argv[]) {
    // check enough arguments
    if (argc < 6 || argc > 7) {
        printUsage(argv[0]);
        return 0;
    }

    // rays
    int ppp = stoi(argv[1]);

    //fotons
    int fotons = stoi(argv[2]);

    // Image
    int width = stoi(argv[3]);
    int height = stoi(argv[4]);

    // Scene
    string sceneName = "default";
    if (argc == 7) {
        sceneName = argv[6];
    }
    Scene scene = createScene(sceneName, (float) width / (float) height);

    if (endsWith(argv[5], ".ppm")) {
        storePPM(string(argv[5]), render(width, height, ppp, fotons,  scene), 255);
    }
    else if (endsWith(argv[5], ".bmp")) {
        storeBMP(string(argv[5]), render(width, height, ppp, fotons, scene));
    }
    else {
        cerr << "Unknown output file extension" << endl;
        return 1;
    }

    return 0;
}
