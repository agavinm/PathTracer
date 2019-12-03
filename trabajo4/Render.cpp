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

                Color pathColor = C_WHITE;
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
                    if (intersection == nullptr) { // Any light founded
                        pathColor = C_BLACK;
                        path = false;
                    }
                    else {
                        HCoord origin = position;
                        position = position + direction * dist; // hit position
                        const HCoord n = normal(intersection->geometry, position);

                        switch (intersection->material.type) {
                            case EMITTER: { // LIGHT
                                pathColor = pathColor * getColor(intersection->material.property.texture, position);
                                path = false;
                                break;
                            }
                            case REFLECTOR: {
                                uniform_real_distribution<float> zeroToOneDistribution(0.0f, 1.0f);
                                float randomZeroToOne, maxKd, maxKs, maxKdPhong, maxKsPhong, pr[3];

                                maxKd = getColor(intersection->material.property.reflectance.kd, position).max();
                                maxKs = getColor(intersection->material.property.reflectance.ks, position).max();
                                maxKdPhong = getColor(intersection->material.property.reflectance.kdPhong, position).max();
                                maxKsPhong = getColor(intersection->material.property.reflectance.ksPhong, position).max();

                                // Russian roulette
                                randomZeroToOne = zeroToOneDistribution(mt);
                                pr[0] = maxKd;
                                pr[1] = maxKs;
                                pr[2] = maxKdPhong + maxKsPhong;
                                if (pr[0] + pr[1] + pr[2] > 0.9f) {
                                    pr[1] = (0.9f / (pr[0] + pr[1] + pr[2]));
                                    pr[0] = maxKd * pr[1];
                                    pr[2] = pr[2] * pr[1];
                                    pr[1] = maxKs * pr[1];
                                }

                                if (randomZeroToOne < pr[0]) { // Perfect refraction case (delta BTDF)
                                    pathColor = pathColor * getColor(intersection->material.property.reflectance.kd, position);
                                    HCoord intermediate = hPoint((origin.x() + position.x()) / 2.0f,
                                            (origin.y() + position.y()) / 2.0f, (origin.z() + position.z()) / 2.0f);

                                    float n1 = VACUUM_REFRACTIVE_INDEX;
                                    bool objectFound = false;
                                    for (unsigned long objectIndex = 0; !objectFound && objectIndex < objects.size(); objectIndex++) {
                                        if (intersect(intermediate, norm(intermediate - P_ZERO), objects[objectIndex]) == 0 &&
                                        objects[objectIndex].material.type == REFLECTOR) {
                                            n1 = objects[objectIndex].material.property.reflectance.n;
                                            objectFound = true;
                                            if (n1 != VACUUM_REFRACTIVE_INDEX) {
                                                cout << "heeh" << endl; //TODO algo raro pasa ya que nunca entra aquí
                                            }
                                        }
                                    }

                                    // https://en.wikipedia.org/wiki/Snell%27s_law
                                    float r = n1 / intersection->material.property.reflectance.n;
                                    float c = dot(-n, direction);
                                    direction = norm(direction * r + n * (r * c - sqrt(1.0f - r * r * (1.0f - c * c))));
                                }
                                else if (randomZeroToOne < pr[0] + pr[1]) { // Perfect specular reflectance case (delta BRDF)
                                    direction = -direction;
                                    direction = direction - (direction - n * dot(direction, n)) * 2.0f;

                                    pathColor = pathColor * getColor(intersection->material.property.reflectance.ks, position);
                                }
                                else if (randomZeroToOne < pr[0] + pr[1] + pr[2]) { // Perfect Phong case (Phong BRDF)
                                    direction = norm(hPoint(2.5f, 2.5f, 5) - position); // WARNING: ONLY A EXAMPLE, TODO

                                    pathColor = pathColor * (getColor(intersection->material.property.reflectance.kdPhong, position) / (float) M_PI
                                            + getColor(intersection->material.property.reflectance.ksPhong, position)
                                            * (intersection->material.property.reflectance.s + 2.0f) / (2.0f * (float) M_PI)
                                            * pow(abs(dot(n, direction)), intersection->material.property.reflectance.s));
                                }
                                else { // Path deaths
                                    pathColor = C_BLACK;
                                    path = false;
                                }

                                break;
                            }
                            default:
                                exit(6);
                        }
                    }
                }
                color = color + pathColor;
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
