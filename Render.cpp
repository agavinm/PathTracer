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
#include "BRDF.hpp"
#include "Random.hpp"
#include "Material.hpp"

using namespace std;

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
 * @return the ray color
 */
Color getLightFromRay(const Scene &scene, HCoord position, HCoord direction) {
    // intialize
    Color rayFactor = C_WHITE; // the color factor (product of brdf and cosines along the path)
    Color directTotal = C_BLACK; // the direct light computed at each step
    int bounce = 1; // number of bounces
    bool path = true; // to stop looping
    stack<const Object *> refractionStack;

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

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const Scene &scene,
                  bool last, Image &image, Progress &progress) {
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

                color = color + getLightFromRay(scene, position, direction);
            }

            // save
            setPixel(image, i, j, color / (float) ppp);
        }
        if (last) progress.step((float) (j - j_ini) * 100.0f / (float) (j_end - j_ini));
    }
}

bool endsWith(const string &original, const string &suffix) {
    return original.compare(original.length() - suffix.length(), suffix.length(), suffix) == 0;
}

void render(int width, int height, int ppp, const Scene &scene, const string &filename) {

    Image hdr = render(width, height, ppp, scene, (int) thread::hardware_concurrency());

    storePPM(filename.substr(0, filename.find_last_of(".")) + "_hdr.ppm", hdr, 65536);

    Image ldr;
    if (isnan(scene.clampCorrection)) {
        if (isnan(scene.gammaCorrection)) {
            // none
            ldr = equalization(hdr);
        } else {
            // gamma only
            ldr = gammaCurve(hdr, scene.gammaCorrection);
        }
    } else {
        if (isnan(scene.gammaCorrection)) {
            // clamp only
            ldr = equalizeAndClamp(hdr, scene.clampCorrection);
        } else {
            // clamp and gamma
            ldr = clampAndGammaCurve(hdr, scene.clampCorrection, scene.gammaCorrection);
        }
    }

    if (endsWith(filename, ".ppm")) {
        storePPM(filename, ldr, 255);
    } else if (endsWith(filename, ".bmp")) {
        storeBMP(filename, ldr);
    } else {
        cerr << "Unknown output file extension" << endl;
    }
}

Image render(int width, int height, int ppp, const Scene &scene, int numThreads) {
    assert(numThreads > 0);

    cout << "[INFO] Rendering " << width << "x" << height << " scene with " << ppp << "ppp (" << scene.objects.size()
         << " objects) (" << numThreads << " threads)" << endl;

    Progress progress;
    thread threads[numThreads];

    progress.start("[INFO] Render");

    Image image = initImage(width, height);

    int j_ini = 0, j_end = height / numThreads;
    for (int n = 0; n < numThreads; n++) {
        threads[n] = thread(renderRegion, j_ini, j_end, width, height, ppp, ref(scene),
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
