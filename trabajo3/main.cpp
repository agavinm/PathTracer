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
#include <random>

#include "HCoord.hpp"
#include "Geometric.hpp"
#include "Scenes.hpp"
#include "Image.hpp"
#include "progress.hpp"

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

    // initialization of utilities
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0.0f, nextafter(1.0f, MAXFLOAT));
    Progress progress;

    // rays
    int ppp = stoi(args[1]);

    // Image
//    int width = 1920; int height = 1080; // HD
    int width = 520;
    int height = 480; // SD
    Image image = initImage(width, height);

    // Scene
    vector<Object> objects = getObjects("XYZ");
    //vector<Object> objects = getObjects("spiral");
    //vector<Object> objects = getObjects("test");
    Camera camera = getCamera((float) width / (float) height);

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << objects.size() << " objects)" << endl;

    progress.start();
    for (int i = 0; i < width; ++i) {
        progress.step(i * 100.0f / width);
        for (int j = 0; j < height; ++j) {
            // foreach pixel
            COLOR color = {0, 0, 0};

            for (int p = 0; p < ppp; ++p) {

                // get initial ray
                HCoord direction = getRay(camera, ((float) i + dist(mt)) / (float) width, ((float) j + dist(mt)) / (float) height);
                HCoord position = camera.origin;

                // find nearest intersection
                const Object *intersection = nullptr;
                float dist = INFINITY;
                for (const Object &object : objects) {
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
    progress.end();

    storePPM("test.ppm", image, 255);

    return 0;
}
