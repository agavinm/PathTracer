/******************************************************************************
 * @file    Render.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 5: Photon mapping
 ******************************************************************************/

#include <thread>
#include <cassert>
#include <cmath>
#include <iostream>
#include "Render.hpp"
#include "Progress.hpp"
#include "Transform.hpp"
#include "PhotonMapping.hpp"
#include "BRDF.hpp"
#include "Random.hpp"
#include "Material.hpp"

using namespace std;

/**
 * Number of bounces where the map will be used.
 *  2 for normal photon mapping
 *  0-1 for no special first bounce
 *  infinity for no photon mapping (path tracing)
 */
const int MAP_AT = 2; // TODO

void launchPhoton(const LightPoint &lightPoint, HCoord direction, vector<Photon> &list, const Scene &scene) {
    // intialize path
    Color photonFactor = lightPoint.color;
    HCoord position = lightPoint.position;
    stack<const Object*> refractionStack;

    int bounce = 1;
    bool path = true;
    while (path) {
        // find nearest intersection
        pair<const Object *, float> object_dist = intersect(position, direction, scene.objects);
        const Object *intersection = object_dist.first;
        float stepDist = object_dist.second;

        if (intersection == nullptr) {
            // no intersection, the photon is discarded
            path = false;
        } else {
            // intersection with an object

            // get properties of bounce
            position = position + direction * stepDist; // hit position

            // check material
            switch (intersection->material.type) {
                case EMITTER: {
                    // LIGHT, the photon dies
                    path = false;
                    break;
                }
                case REFLECTOR: {
                    // a reflector (a non-emitter)

                    // get BRDF & next ray of the event
                    EVENT event = getRandomEvent(*intersection, position);
                    HCoord nextDirection = getNewDirection(event, position, direction, *intersection,
                            refractionStack, scene.refractiveIndex);

                    if (event == PHONG_DIFFUSE || event == PHONG_SPECULAR) {
                        // only on phong cases

                        if (bounce >= MAP_AT) {
                            // ready to save photon in map
                            list.push_back({position, direction, photonFactor, intersection});
                        }
                    }

                    if (event == DEAD) {
                        // if the event is to kill, the photon dies
                        path = false;
                    }

                    if (path) {
                        // if the path is still active, prepare for next bounce
                        bounce++;

                        // update factor with the brdf
                        photonFactor = photonFactor
                                      * getBRDF(event, direction, nextDirection, position, *intersection);

                        // next direction
                        direction = nextDirection;
                    }

                    break;
                }
            }

        }

        if (photonFactor.max() <= 0.0f) {
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
 * @param globalPhotonMap photonmap to use for photonmapping
 * @return the ray color
 */
Color getLightFromRay(const Scene &scene, HCoord position, HCoord direction, const PhotonMap &globalPhotonMap) {
    // intialize
    Color rayFactor = C_WHITE; // the color factor (product of brdf and cosines along the path)
    Color directTotal = C_BLACK; // the direct light computed at each step
    int bounce = 1; // number of bounces
    bool path = true; // to stop looping
    stack<const Object*> refractionStack;

    while (path) {
        // find nearest intersection
        pair<const Object*, float> object_dist = intersect(position, direction, scene.objects);
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
                    HCoord nextDirection = getNewDirection(event, position, direction, *intersection,
                            refractionStack, scene.refractiveIndex);

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
                                               * getBRDF(event, norm(lightVect), -direction, position, *intersection)
                                               * rayFactor
                                               / (lightDist * lightDist);

                                directTotal = directTotal + direct;
                            }
                        }

                        if (bounce >= MAP_AT - 1) {
                            // get light from map
                            rayFactor = rayFactor
                                        * globalPhotonMap.getColorFromMap(position, direction, intersection);
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
                                    * getBRDF(event, -nextDirection, -direction, position, *intersection);

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
    return rayFactor + directTotal / (float) bounce;

}

void launchPhotons(int photons, const Scene &scene, bool last, Progress &progress, PhotonMap &globalPhotonMap) {
    vector<Photon> photonList;

    for (int i = 0; i < photons; ++i) {
        // launch each photon

        // get random light
        LightPoint point = scene.lightPoints.at(random_zero_n(scene.lightPoints.size() - 1));

        // get random direction
        double theta = 2 * M_PI * random_zero_one();
        double phi = acos(1 - 2 * random_zero_one());
        HCoord direction = hVector(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));
        // launch
        launchPhoton(point, direction, photonList, scene);

        if (last) progress.step((float) i * 100.0f / (float) (photons));
    }

    // finished, add to global map
    globalPhotonMap.addAll(photonList);
}

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const Scene &scene,
        const PhotonMap &globalPhotonMap, bool last, Image &image, Progress &progress) {
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

                color = color + getLightFromRay(scene, position, direction, globalPhotonMap);
            }

            // save
            setPixel(image, i, j, color / (float) ppp);
        }
        if (last) progress.step((float) (j - j_ini) * 100.0f / (float) (j_end - j_ini));
    }
}

Image render(int width, int height, int ppp, int photons, const Scene &scene) {
    return render(width, height, ppp, photons, scene, (int) thread::hardware_concurrency());
}

Image render(int width, int height, int ppp, int photons, const Scene &scene, int numThreads) {
    assert(numThreads > 0);

    cout << "[INFO] Creating PhotonMap of " << photons << " photons " <<
         "(" << scene.objects.size() << " objects) " <<
         "(" << numThreads << " threads) " <<
         "(" << scene.lightPoints.size() << " lights)" <<
         endl;

    Progress progress;
    thread threads[numThreads];

    PhotonMap globalPhotonMap;

    if (!scene.lightPoints.empty()) {
        progress.start("[INFO] PhotonMapping");

        for (int n = 0; n < numThreads; n++) {
            threads[n] = thread(launchPhotons, ceil(photons / numThreads), ref(scene),
                                n == numThreads - 1, ref(progress), ref(globalPhotonMap));
        }

        for (int n = 0; n < numThreads; n++) {
            threads[n].join(); // wait thread n ends
        }

        progress.step(99.99f);

        globalPhotonMap.markToRead();

        progress.end();

    } else {
        cout << "[INFO] No lightpoints, skipped photonMapping" << endl;
    }

    //-----------------------------------

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << scene.objects.size()
         << " objects) (" << numThreads << " threads)" << endl;

    progress.start("[INFO] Render");

    Image image = initImage(width, height);

    int j_ini = 0, j_end = height / numThreads;
    for (int n = 0; n < numThreads; n++) {
        threads[n] = thread(renderRegion, j_ini, j_end, width, height, ppp, ref(scene), ref(globalPhotonMap),
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
