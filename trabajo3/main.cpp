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

bool endsWith(const string &original, const string &suffix) {
    return original.compare(original.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void printUsage(const string &name) {
    cout << "Usage: " << name << " <ppp>" << " <file output>" << endl
         << "* <file output> needs to be one of" << endl
         << "       - .ppm" << endl
         << "       - .bmp" << endl;
}

int main(int argc, char *argv[]) {
    // check enough arguments
    if (argc < 3) {
        printUsage(argv[0]);
        return 0;
    }

    // rays
    int ppp = stoi(argv[1]);

    // Image
    //int width = 1920; int height = 1080; // HD
    //int width = 520; int height = 480; // SD
    int width = 500; int height = 500; //square

    // Scene
    //vector<Object> objects = getObjects("XYZ");
    vector<Object> objects = getObjects("spiral");
    //vector<Object> objects = getObjects("test");
    Camera camera = getCamera((float) width / (float) height);

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << objects.size() << " objects)" << endl;

    if (endsWith(argv[2], ".ppm")) {
        storePPM(argv[2], render(width, height, ppp, objects, camera), 255);

    } else if (endsWith(argv[2], ".bmp")) {
        storeBMP(argv[2], render(width, height, ppp, objects, camera));

    } else {
        cerr << "Unknown output file extension" << endl;
    }

    return 0;
}
