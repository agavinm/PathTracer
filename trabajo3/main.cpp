/******************************************************************************
 * @file    main.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <iostream>
#include "Scenes.hpp"
#include "Image.hpp"
#include "Render.hpp"

using namespace std;

void printUsage(const string &name) {
    cout << "Usage: " << name << " <ppp> <scene> <output format>" << endl
         << "* <scene> can be a .ply file or a predefined scene:" << endl
         << "       test" << endl
         << "       spiral" << endl
         << "       XYZ" << endl
         << "* <output format> needs to be" << endl
         << "       ppm" << endl
         << "       bmp" << endl;
}

int main(int argc, char *argv[]) {
    // check enough arguments
    if (argc < 4) {
        printUsage(argv[0]);
        return 0;
    }

    // rays
    int ppp = stoi(argv[1]);

    // Image
    //int width = 1920; int height = 1080; // HD
    //int width = 520; int height = 480; // SD
    int width = 500; int height = 500; //square
    //int width = 1000; int height = 500; //strip

    // Scene
    vector<Object> objects = getObjects(argv[2]);;
    Camera camera = getCamera((float) width / (float) height);

    if (string(argv[3]) == "ppm") {
        storePPM(string(argv[2]) + ".ppm", render(width, height, ppp, objects, camera), 255);

    }
    else if (string(argv[3]) == "bmp") {
        storeBMP(string(argv[2]) + ".bmp", render(width, height, ppp, objects, camera));

    }
    else {
        cerr << "Unknown output file extension" << endl;
        return 1;
    }

    return 0;
}
