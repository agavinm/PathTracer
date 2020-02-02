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
    cout << "Usage: " << name << " <ppp> <width> <height> <file name> <scene>" << endl
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

    render(width, height, ppp, scene, argv[4]);

    return 0;
}
