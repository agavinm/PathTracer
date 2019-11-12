/******************************************************************************
 * @file    Render.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <thread>
#include <cassert>
#include <cmath>
#include <random>
#include "Render.hpp"
#include "Progress.hpp"

using namespace std;

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const vector<Object> &objects, const Camera &camera, bool last, Image &image, Progress &progress) {
    // initialization of utilities
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0.0f, nextafter(1.0f, MAXFLOAT));

    for (int j = j_ini; j < j_end; ++j) {
        for (int i = 0; i < width; ++i) {
            // foreach pixel
            COLOR color = C_BLACK;

            for (int p = 0; p < ppp; ++p) {
                // get initial ray
                HCoord direction = getRay(camera, ((float) i + dist(mt)) / (float) width, ((float) j + dist(mt)) / (float) height);
                //HCoord direction = getRay(camera, (float) i / (float) width, (float) j / (float) height);
                HCoord position = camera.origin;

                // find nearest intersection
                const Object *intersection = nullptr;
                float dist = INFINITY;
                for (const Object &object : objects) {
                    float obj_dist = intersect(position, direction, object);
                    if (obj_dist > -EPS && obj_dist < dist) {
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
            setPixel(image, i, j, color / (float) ppp);

        }
        if (last) progress.step((float) (j - j_ini) * 100.0f / (float) (j_end - j_ini));
    }
}

Image render(int width, int height, int ppp, const vector<Object> &objects, const Camera &camera) {
    return render(width, height, ppp, objects, camera, (int) thread::hardware_concurrency());
}

Image render(int width, int height, int ppp, const vector<Object> &objects, const Camera &camera, int numThreads) {
    assert(numThreads > 0);

    Progress progress;
    progress.start("[INFO] Render");

    Image image = initImage(width, height);

    thread threads[numThreads];

    int j_ini = 0, j_end = height / numThreads;
    for (int n = 0; n < numThreads; n++) {

        threads[n] = thread(renderRegion, j_ini, j_end, width, height, ppp, ref(objects), ref(camera),
                            n == numThreads - 1, ref(image), ref(progress));

        j_ini = j_end;
        if (n == numThreads - 2) j_end = height; // next is last iteration
        else j_end += height / numThreads;
    }

    for (int n = 0; n < numThreads; n++) {
        threads[n].join(); // wait thread n ends
    }

    progress.end();

    return image;
}
