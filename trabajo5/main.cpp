/******************************************************************************
 * @file    main.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 5: Photon mapping
 ******************************************************************************/

#include <iostream>
#include "Render.hpp"

using namespace std;

void printUsage(const string &name) {
    cout << "Usage: " << name << " <ppp> <photons> <width> <height> <file name> <scene>" << endl
         << "* <file name> needs to be one of" << endl
         << "       - .ppm" << endl
         << "       - .bmp" << endl
         << "OPTIONAL* <scene> can be a triangular .ply file or a predefined scene." << endl;
    printScenes();
}

int main(int argc, char *argv[]) {
#ifdef USING_DEBUG
    // debug code
    cout << "!!!!! USING SLOW DEBUG CODE !!!!!" << endl;
#endif

    // check enough arguments
    if (argc < 6 || argc > 7) {
        printUsage(argv[0]);
        return 0;
    }

    // rays
    int ppp = stoi(argv[1]);

    //photons
    int photons = stoi(argv[2]);

    // Image
    int width = stoi(argv[3]);
    int height = stoi(argv[4]);

    // Scene
    string sceneName = "default";
    if (argc == 7) {
        sceneName = argv[6];
    }
    Scene scene = createScene(sceneName, (float) width / (float) height);

    render(width, height, ppp, photons, scene, argv[5]);

    return 0;
}
