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
#include "Material.hpp"

using namespace std;

/**
 * Number of bounces where the map will be used.
 *  2 for normal foton mapping
 *  0-1 for no special first bounce
 *  infinity for no foton mapping (path tracing)
 */
const int MAP_AT = 2;

void launchFoton(const LightPoint &lightPoint, HCoord direction, vector<Foton> &list, const Scene &scene) {

    // intialize path
    Color fotonFactor = lightPoint.color;
    HCoord position = lightPoint.position;

    int bounce = 1;
    bool path = true;
    while (path) {

        // find nearest intersection
        pair<const Object *, float> object_dist = intersect(position, direction, scene.objects);
        const Object *intersection = object_dist.first;
        float stepDist = object_dist.second;

        if (intersection == nullptr) {
            // no intersection, the foton is discarded
            path = false;
        } else {
            // intersection with an object

            // get properties of bounce
            position = position + direction * stepDist; // hit position
            HCoord n = normal(intersection->geometry, position);

            // update factor with the geometry component
            fotonFactor = fotonFactor * abs(dot(n, direction));

            // check material
            switch (intersection->material.type) {
                case EMITTER: {
                    // LIGHT, the foton dies
                    path = false;
                    break;
                }
                case REFLECTOR: {
                    // a reflector (a non-emitter)

                    // get BRDF & next ray of the event
                    EVENT event = getRandomEvent(*intersection, position);
                    HCoord nextDirection = getNewDirection(event, position, direction, *intersection);

                    if (event == PHONG_DIFFUSE || event == PHONG_SPECULAR) {
                        // only on phong cases

                        if (bounce >= MAP_AT) {
                            // ready to save foton in map
                            list.push_back({position, direction, fotonFactor, intersection});
                        }
                    }

                    if (event == DEAD) {
                        // if the event is to kill, the foton dies
                        path = false;
                    }

                    if (path) {
                        // if the path is still active, prepare for next bounce
                        bounce++;

                        // update factor with the brdf
                        fotonFactor = fotonFactor
                                      * getBRDF(event, direction, nextDirection, position, *intersection);

                        // next direction
                        direction = nextDirection;
                    }

                    break;
                }
            }

        }

        if (fotonFactor.max() <= 0.0f) {
            // avoid following paths with no light
            path = false;
        }
    }

}

bool isLightVisible(const LightPoint &lightPoint, const HCoord &position, const vector<Object> &objects) {
    HCoord lightVect = position - lightPoint.position;
    float lightDist = intersect(lightPoint.position, norm(lightVect), objects).second;
    return lightDist >= mod(lightVect) - EPS;
}

/**
 * Returns the light towards a ray
 * @param scene the scene to use
 * @param position position of start of the ray
 * @param direction direction of the ray, from 'position' towards the scene
 * @param globalFotonMap fotonmap to use for fotonmapping
 * @return the ray color
 */
Color getLightFromRay(const Scene &scene, HCoord position, HCoord direction, const FotonMap &globalFotonMap) {

    // intialize
    Color rayFactor = C_WHITE; // the color factor (product of brdf and cosines along the path)
    Color directTotal = C_BLACK; // the direct light computed at each step
    int bounce = 1; // number of bounces
    bool path = true; // to stop looping

    while (path) {

        // find nearest intersection
        pair<const Object *, float> object_dist = intersect(position, direction, scene.objects);
        const Object *intersection = object_dist.first;
        float dist = object_dist.second;

        if (intersection == nullptr) {
            // no intersection, return the 'void color'
            rayFactor = C_BLACK;
            path = false;

        } else {
            // intersection with an object

            // get properties of bounce
            position = position + direction * dist; // update position with hit
            HCoord n = normal(intersection->geometry, position); // normal of hit point

            // check material
            switch (intersection->material.type) {
                case EMITTER: {
                    // LIGHT, end of path
                    rayFactor = rayFactor
                                * getColor(intersection->material.property.texture, position); // update factor with light color
                    path = false; // end
                    break;
                }
                case REFLECTOR: {
                    // a reflector (a non-emitter)

                    // get BRDF & next ray of the event
                    EVENT event = getRandomEvent(*intersection, position);
                    HCoord nextDirection = getNewDirection(event, position, direction, *intersection);

                    if (event == PHONG_DIFFUSE || event == PHONG_SPECULAR) {
                        // only on phong cases

                        // get direct light
                        for (const LightPoint &lightPoint : scene.lightPoints) {
                            // foreach light
                            if (isLightVisible(lightPoint, position, scene.objects)) {

                                // if visible, compute path light
                                HCoord lightVect = position - lightPoint.position;
                                float lightDist = mod(lightVect);
                                Color direct = lightPoint.color
                                               * getBRDF(event, lightVect, -direction, position, *intersection)
                                               * abs(dot(n, lightVect))
                                               * rayFactor
                                               / (lightDist * lightDist);

                                directTotal = directTotal + direct;
                            }
                        }

                        if (bounce >= MAP_AT - 1) {
                            // get light from map
                            rayFactor = rayFactor
                                        * globalFotonMap.getColorFromMap(position, direction, intersection)
                                        * abs(dot(n, direction));
                            path = false;
                        }
                    }

                    if (event == DEAD) {
                        // if the event is to kill, the path ends
                        rayFactor = C_BLACK;
                        path = false;
                    }

                    if (path) {
                        // if the path is still active, prepare for next bounce
                        bounce++;

                        // update factor with the brdf and the geometry component
                        rayFactor = rayFactor
                                    * getBRDF(event, direction, nextDirection, position, *intersection)
                                    * abs(dot(n, direction));

                        // next direction
                        direction = nextDirection;
                    }
                    break;
                }
            }

        }

        if (rayFactor.max() <= 0.0f) {
            // avoid following paths with no light
            path = false;
        }
    }

    // return light
    return rayFactor + directTotal / bounce;

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

    Progress progress;
    thread threads[numThreads];

    FotonMap globalFotonMap;

    if (not scene.lightPoints.empty()) {

        progress.start("[INFO] FotonMapping");

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

    } else {
        cout << "[INFO] No lightpoints, skipped fotonMapping" << endl;
    }

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

    if (isnan(scene.clampCorrection)) {
        if (isnan(scene.gammaCorrection)) {
            // none
            return equalization(image);
        } else {
            // gamma only
            return gammaCurve(image, scene.gammaCorrection);
        }
    } else {
        if (isnan(scene.gammaCorrection)) {
            // clamp only
            return equalizeAndClamp(image, scene.clampCorrection);
        } else {
            // clamp and gamma
            return clampAndGammaCurve(image, scene.clampCorrection, scene.gammaCorrection);
        }
    }
}
