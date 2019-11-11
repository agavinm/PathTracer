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

void renderRegion(int i_ini, int i_end, int width, int height, int ppp,
        const vector<Object> &objects, const Camera &camera, bool last, vector<COLOR> &pixels, Progress &progress) {
    // initialization of utilities
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0.0f, nextafter(1.0f, MAXFLOAT));

    for (int i = i_ini; i < i_end; ++i) {
        for (int j = 0; j < height; ++j) {
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
            pixels.push_back(color / (float) ppp);

            if (last) progress.step((float) (i - i_ini) * 100.0f / (float) (i_end - i_ini));
        }
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
    vector<COLOR> pixels[numThreads]; // return thread vector array

    int i_ini = 0, i_end = width / numThreads;
    for (int n = 0; n < numThreads; n++) {
        pixels[n].reserve((i_end - i_ini) * height);

        threads[n] = thread(renderRegion, i_ini, i_end, width, height, ppp, ref(objects), ref(camera),
                n == numThreads - 1, ref(pixels[n]), ref(progress));

        i_ini = i_end;
        if (n == numThreads - 2) i_end = width; // next is last iteration
        else i_end += width / numThreads;
    }

    int p = 0;
    for (int n = 0; n < numThreads; n++) {
        threads[n].join(); // wait thread n ends

        for (auto i : pixels[n]) {
            setPixel(image, p, i); // save result of thread n into image
            p++;
        }
    }

    progress.end();

    return image;
}
