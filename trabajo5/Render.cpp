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
#include "FotonMapping.hpp"
#include "BRDF.hpp"
#include "Random.hpp"

using namespace std;

void launchFoton(const LightPoint &lightPoint, HCoord direction, vector<Foton> &list, const Scene &scene) {
    Color color = lightPoint.color;
    float pathLength = 0.0f;
    HCoord position = lightPoint.position;

    bool first = true; // first intersections is not saved
    bool path = true;
    while (path) {
        // find nearest intersection
        pair<const Object *, float> object_dist = intersect(position, direction, scene.objects);
        const Object *intersection = object_dist.first;
        float dist = object_dist.second;

        if (intersection == nullptr) {
            // void: bye bye foton
            color = C_BLACK;
        } else {
            // intersection with an object
            HCoord origin = position;
            position = position + direction * dist; // hit position
            HCoord n = normal(intersection->geometry, position);
            pathLength += dist;

            switch (intersection->material.type) {
                case EMITTER: { // LIGHT
                    throw invalid_argument("With fotonMapping emitters are not supported");
                }
                case REFLECTOR: {

                    pair<Color, HCoord> result;
                    switch(getRandomEvent(*intersection, position)){
                        case REFRACTION:
                            result = refraction(scene, origin, position, direction, n, *intersection);
                            break;
                        case REFLECTION:
                            result = reflection(position, direction, n, *intersection);
                            break;
                        case PHONG:
                            result = phong(scene, position, direction, n, *intersection);
                            break;
                        case DEAD:
                            color = C_BLACK;
                            break;
                    }

                    color = color * result.first;

                    // save foton
                    if (first) {
                        first = false;
                    } else {
                        list.push_back({position, direction, color, pathLength, intersection});
                    }

                    color = color * abs(dot(n, result.second));
                    direction = result.second;

                    break;
                }
                default: {
                    exit(6);
                }
            }

        }

        if (color.max() <= 0.0f) {
            // avoid following paths with no light
            path = false;
        }
    }

}

Color getLightFromRay(const Scene &scene, HCoord position, HCoord direction, const FotonMap &globalFotonMap) {

    // find nearest intersection
    pair<const Object *, float> object_dist = intersect(position, direction, scene.objects);
    const Object *intersection = object_dist.first;
    float dist = object_dist.second;

    if (intersection == nullptr) {
        // void: nothing
        return C_BLACK;
    } else {
        // found object
        position = position + direction * dist; // hit position

        Color direct = C_BLACK;
        // get direct light
        for (const LightPoint &lightPoint : scene.lightPoints) {
            HCoord lightVect = position - lightPoint.position;
            float lightDist = intersect(lightPoint.position, norm(lightVect), scene.objects).second;
            if (lightDist > mod(lightVect) - EPS) {
                float pathLightDist = dist + lightDist;
                direct = direct + lightPoint.color * getColor(intersection->material.property.texture, position) * abs(dot(normal(intersection->geometry, position), norm(lightVect))) / (pathLightDist * pathLightDist);
            }
        }

        // calculate light
        return direct + globalFotonMap.getColorFromMap(position, direction, dist, intersection);
    }

}

void launchFotons(int fotons, const Scene &scene, bool last, Progress &progress, FotonMap &globalFotonMap) {
    vector<Foton> fotonList;

    for (int i = 0; i < fotons; ++i) {
        // launch each foton

        // get random light
        LightPoint point = scene.lightPoints.at(random_zero_n(scene.lightPoints.size() - 1));

        // get random direction
        double theta = 2 * M_PI * random_zero_one();
        double phi = acos(1 - 2 * random_zero_one());
        HCoord direction = hVector(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));

        // launch
        launchFoton(point, direction, fotonList, scene);

        if (last) progress.step((float) i * 100.0f / (float) (fotons));
    }

    // finished, add to global map
    globalFotonMap.addAll(fotonList);
}

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const Scene &scene, const FotonMap &globalFotonMap, bool last, Image &image, Progress &progress) {

    for (int j = j_ini; j < j_end; ++j) {
        for (int i = 0; i < width; ++i) {
            // foreach pixel
            Color color = C_BLACK;

            for (int p = 0; p < ppp; ++p) {
                // get initial ray
                HCoord direction = norm(getRay(scene.camera, ((float) i + random_zero_one()) / (float) width,
                                               ((float) j + random_zero_one()) /
                                               (float) height)); // should be a normalized ray
                HCoord position = scene.camera.origin;

                color = color + getLightFromRay(scene, position, direction, globalFotonMap);
            }

            // save
            setPixel(image, i, j, color / (float) ppp);
        }
        if (last) progress.step((float) (j - j_ini) * 100.0f / (float) (j_end - j_ini));
    }
}

Image render(int width, int height, int ppp, int fotons, const Scene &scene) {
    return render(width, height, ppp, fotons, scene, (int) thread::hardware_concurrency());
}

Image render(int width, int height, int ppp, int fotons, const Scene &scene, int numThreads) {
    assert(numThreads > 0);

    cout << "[INFO] Creating FotonMap of " << fotons << " fotons " <<
         "(" << scene.objects.size() << " objects) " <<
         "(" << numThreads << " threads) " <<
         "(" << scene.lightPoints.size() << " lights)" <<
         endl;

    FotonMap globalFotonMap;

    Progress progress;
    progress.start("[INFO] FotonMapping");

    thread threads[numThreads];

    for (int n = 0; n < numThreads; n++) {

        threads[n] = thread(launchFotons, ceil(fotons / numThreads), ref(scene),
                            n == numThreads - 1, ref(progress), ref(globalFotonMap));
    }

    for (int n = 0; n < numThreads; n++) {
        threads[n].join(); // wait thread n ends
    }

    progress.step(99.99f);

    globalFotonMap.markToRead();

    progress.end();

    //-----------------------------------

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << scene.objects.size()
         << " objects) (" << numThreads << " threads)" << endl;

    progress.start("[INFO] Render");

    Image image = initImage(width, height);

    int j_ini = 0, j_end = height / numThreads;
    for (int n = 0; n < numThreads; n++) {

        threads[n] = thread(renderRegion, j_ini, j_end, width, height, ppp, ref(scene), ref(globalFotonMap),
                            n == numThreads - 1, ref(image), ref(progress));

        j_ini = j_end;
        if (n == numThreads - 2) j_end = height; // next is last iteration
        else j_end += height / numThreads;
    }

    for (int n = 0; n < numThreads; n++) {
        threads[n].join(); // wait thread n ends
    }

    progress.end();

    return gammaCurve(image, scene.gammaCorrection);
}
