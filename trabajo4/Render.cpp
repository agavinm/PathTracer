/******************************************************************************
 * @file    Render.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include <thread>
#include <cassert>
#include <cmath>
#include <random>
#include <iostream>
#include "Render.hpp"
#include "Progress.hpp"
#include "Transform.hpp"
#include "Material.hpp"
#include "Geometry.hpp"

using namespace std;

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const vector<Object> &objects,
                  const Camera &camera, bool last, Image &image, Progress &progress) {
    // initialization of utilities
    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<float> dist(0.0f, nextafter(1.0f, MAXFLOAT));

    for (int j = j_ini; j < j_end; ++j) {
        for (int i = 0; i < width; ++i) {
            // foreach pixel
            Color color = C_BLACK;

            for (int p = 0; p < ppp; ++p) {
                // get initial ray
                HCoord direction = norm(getRay(camera, ((float) i + dist(mt)) / (float) width,
                                               ((float) j + dist(mt)) / (float) height)); // should be a normalized ray
                HCoord position = camera.origin;

                bool path = true;
                while (path) {
                    // find nearest intersection
                    const Object *intersection = nullptr;
                    float dist = INFINITY;
                    for (const Object &object : objects) {
                        float obj_dist = intersect(position, direction, object);
                        if (obj_dist > EPS && obj_dist < dist) {
                            intersection = &object;
                            dist = obj_dist;
                        }
                    }

                    // Get color
                    if (intersection == nullptr) {
                        path = false;
                    }
                    else {
                        position = position + direction * dist; // hit position

                        switch (intersection->material.type) {
                            case EMITTER: {
                                color = color + getColor(intersection->material.property.texture, position);
                                path = false;
                                break;
                            }
                            case REFLECTOR: {
                                Color pathColor = C_BLACK;
                                uniform_real_distribution<float> zeroToOneDistribution(0.0f, 1.0f);
                                float randomZeroToOne, maxKd, maxKs, pr[3];

                                maxKd = getColor(intersection->material.property.reflectance.kd, position).max();
                                maxKs = getColor(intersection->material.property.reflectance.ks, position).max();

                                // Russian roulette
                                randomZeroToOne = zeroToOneDistribution(mt);
                                pr[0] = intersection->material.property.reflectance.kp;
                                pr[1] = maxKs;
                                pr[2] = maxKd;
                                if (pr[0] + pr[1] + pr[2] > 0.9f) {
                                    pr[2] = (0.9f / (pr[0] + maxKs + maxKs));
                                    pr[0] = pr[0] * pr[2];
                                    pr[1] = maxKd * pr[2];
                                    pr[2] = maxKs * pr[2];
                                }

                                if (randomZeroToOne < pr[0]) {
                                    pathColor = {0.1f, 0.1f, 0.1f}; // WARNING: ONLY IS A EXAMPLE
                                    // Phong BRDF case
                                    //TODO
                                }
                                else if (randomZeroToOne < pr[0] + pr[1]) {
                                    pathColor = {0.2f, 0.2f, 0.2f}; // WARNING: ONLY IS A EXAMPLE
                                    // Perfect specular reflectance case (delta BRDF)
                                    //TODO
                                }
                                else if (randomZeroToOne < pr[0] + pr[1] + pr[2]) {
                                    pathColor = {0.05f, 0.05f, 0.05f}; // WARNING: ONLY IS A EXAMPLE
                                    // Perfect refraction case (delta BTDF)
                                    //TODO
                                }
                                else {
                                    // Path deaths
                                    path = false;
                                }

                                color = color + pathColor;

                                // New random direction without going through the surface (+- 90º from object normal)
                                for (int c = 0; c < 3; c++) {
                                    randomZeroToOne = zeroToOneDistribution(mt);
                                    if (direction.e[c] == 0.0f || (direction.e[c] < 0.0f && randomZeroToOne < 0.0f) ||
                                            (direction.e[c] > 0.0f && randomZeroToOne > 0.0f)) {
                                        direction.e[c] = randomZeroToOne;
                                    }
                                    else {
                                        direction.e[c] = randomZeroToOne * -1.0f;
                                    }
                                }
                                direction = norm(direction);

                                break;
                            }
                            default:
                                exit(6);
                        }
                    }
                }
            }

            // save
            setPixel(image, i, j, color / (float) ppp);
        }
        if (last) progress.step((float) (j - j_ini) * 100.0f / (float) (j_end - j_ini));
    }
}

Image render(int width, int height, int ppp, const Scene &scene) {
    return render(width, height, ppp, scene, (int) thread::hardware_concurrency());
}

Image render(int width, int height, int ppp, const Scene &scene, int numThreads) {
    assert(numThreads > 0);

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << scene.objects.size()
         << " objects) (" << numThreads << " threads)" << endl;

    Progress progress;
    progress.start("[INFO] Render");

    Image image = initImage(width, height);

    thread threads[numThreads];

    int j_ini = 0, j_end = height / numThreads;
    for (int n = 0; n < numThreads; n++) {

        threads[n] = thread(renderRegion, j_ini, j_end, width, height, ppp, ref(scene.objects), ref(scene.camera),
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
