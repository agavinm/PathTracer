//******************************************************************************
// File:   main.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//******************************************************************************

#include <cmath>
#include <vector>

#include "HCoord.hpp"
#include "Geometric.hpp"
#include "Scenes.hpp"
#include "Image.hpp"

using namespace std;

int main() {


    int width = 100;
    int height = 100;

    vector<Object> objects = getObjects();

    Camera camera = getCamera((float) width / (float) height);

    Image image = initImage(width, height);

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {

            HCoord direction = getRay(camera, (float) i / (float) width, (float) j / (float) height);
            HCoord position = camera.origin;

            Object *intersection = nullptr;
            float dist = INFINITY;
            for (Object &object : objects) {
                if (posDist(position, direction, object) < dist) {
                    intersection = &object;
                }
            }

            float r, g, b;
            if (intersection == nullptr) {
                // no object
                r = g = b = 0;
            } else {
                // object
                switch (intersection->material.type) {
                    case EMITTER: {
                        MATERIAL_EMMITER data = intersection->material.material_data.emitter_data;
                        r = data.r;
                        g = data.g;
                        b = data.b;
                        break;
                    }
                    default:
                        exit(1);
                }
            }

            setPixel(image, i, j, r, g, b);
        }
    }

    storePPM("test.ppm", image, 255);

    return 0;
}
