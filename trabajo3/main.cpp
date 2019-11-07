//******************************************************************************
// File:   main.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//******************************************************************************

#include <cmath>
#include <vector>
#include <iostream>

#include "HCoord.hpp"
#include "Geometric.hpp"
#include "Scenes.hpp"
#include "Image.hpp"

#define RAND(max) ( (float)rand() / (float)(RAND_MAX) * (float) (max) )

using namespace std;

void printUsage(const string &name) {
    cout << "Usage: " << name << " <ppp>" << endl;
}

int main(int argc, char *argv[]) {

    // for convenience
    vector<string> args(argv, argv + argc);

    // check enough arguments
    if (argc < 2) {
        printUsage(args[0]);
        return 0;
    }

    // rays
    int ppp = stoi(args[1]);

    // Image
//    int width = 1920; int height = 1080; // HD
    int width = 520;
    int height = 480; // SD
    Image image = initImage(width, height);

    // Scene
    vector<Object> objects = getObjects();
    Camera camera = getCamera((float) width / (float) height);

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << objects.size() << " objects)" << endl;

    for (int i = 0; i < width; ++i) {
        cout << "\r> Render status: " << i * 100 / width << "%     " << flush;
        for (int j = 0; j < height; ++j) {
            // foreach pixel
            COLOR color = {0, 0, 0};

            for (int p = 0; p < ppp; ++p) {

                // get initial ray
                HCoord direction = getRay(camera, ((float) i + RAND(1)) / (float) width, ((float) j + RAND(1)) / (float) height);
                HCoord position = camera.origin;

                // find nearest intersection
                Object *intersection = nullptr;
                float dist = INFINITY;
                for (Object &object : objects) {
                    float obj_dist = intersect(position, direction, object);
                    if (obj_dist > -1e-6 && obj_dist < dist) {
                        intersection = &object;
                        dist = obj_dist;
                    }
                }

                // get color
                if (intersection == nullptr) {
                    // no object
                    // pixel += 0
                } else {
                    // object
                    switch (intersection->material.type) {
                        case EMITTER: {
                            color = color + intersection->material.data.emitter;
                            break;
                        }
                        default:
                            exit(6);
                    }
                }
            }

            // save
            setPixel(image, i, j, color / ppp);
        }

    }
    cout << "\r[INFO] Render completed                   " << endl;

    storePPM("test.ppm", image, 255);

    return 0;
}
