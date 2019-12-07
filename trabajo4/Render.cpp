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

using namespace std;

pair<Color, HCoord> reflection(const HCoord &position, const HCoord &direction, const HCoord &n, const Object &intersection) {
    pair<Color, HCoord> result;

    result.first = getColor(intersection.material.property.reflectance.ks, position);

    result.second = norm(direction - (direction - n * dot(direction, n)) * 2.0f);

    return result;
}

pair<Color, HCoord> refraction(const Scene &scene, const HCoord &origin, const HCoord &position, const HCoord &direction,
        HCoord n, const Object &intersection) {
    pair<Color, HCoord> result;

    result.first = getColor(intersection.material.property.reflectance.kd, position);

    HCoord intermediate = hPoint((origin.x() + position.x()) / 2.0f,
                                 (origin.y() + position.y()) / 2.0f, (origin.z() + position.z()) / 2.0f);
    float n1 = scene.refractiveIndex, n2 = scene.refractiveIndex;
    const Object *o = nullptr;
    bool insideOf_o = false;
    for (unsigned long objectIndex = 0; !insideOf_o && objectIndex < scene.objects.size(); objectIndex++) {
        o = &scene.objects[objectIndex];
        if (o->material.type == REFLECTOR && o->type == OBJECT_3D && isInside(intermediate, *o)) {
            // Ray is inside an object
            n1 = o->n;
            insideOf_o = true;
        }
    }
    if (insideOf_o) {
        // Ray is inside an object
        n = -n;
        switch (o->geometry.type) {
            case SPHERE: {
                if (mod(position - o->geometry.data.sphere.center) == o->geometry.data.sphere.radius) {
                    // Ray will come out of the sphere
                    bool objectFound = false;
                    for (unsigned long objectIndex = 0; !objectFound && objectIndex < scene.objects.size(); objectIndex++) {
                        const Object *outside = &scene.objects[objectIndex];
                        if (outside->material.type == REFLECTOR && outside->type == OBJECT_3D &&
                            isInside(position, *outside) && outside != o) {
                            // Ray will enter an outside object
                            n2 = outside->n;
                            objectFound = true;
                        }
                    }
                }
                else {
                    // Ray will enter another object
                    n2 = intersection.n;
                }

                break;
            }
            default: {
                exit(6);
            }
        }
    }
    else {
        // Ray will enter an object
        n2 = intersection.n;
    }

    // https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
    float r = n1 / n2;
    float c = dot(-n, direction);
    float radicand = 1.0f - r * r * (1.0f - c * c);
    if (radicand < 0.0f) {
        // Only happens reflection because the sine of the angle of refraction is required to be greater than one.
        // https://en.wikipedia.org/wiki/Snell%27s_law#Total_internal_reflection_and_critical_angle
        return reflection(position, direction, n, intersection);
    }
    else {
        result.second = norm(direction * r + n * (r * c - sqrt(1.0f - r * r * (1.0f - c * c))));

        return result;
    }
}

pair<Color, HCoord> phong(const Scene &scene, const HCoord &position, const HCoord &direction, const HCoord &n,
        const Object &intersection, mt19937 &mt) {
    pair<Color, HCoord> result;

    HCoord ref = norm(direction - (direction - n * dot(direction, n)) * 2.0f);

    HCoord Z;
    HCoord Y;
    HCoord X;

    if (abs(dot(direction, n)) < EPS) {
        Z = -direction;
        Y = n;
        X = cross(Y, Z);
    }
    else {
        X = norm(cross(direction, ref));
        Y = ref;
        Z = cross(Y, X);
    }

    uniform_real_distribution<float> dist(0.0f, 1.0f);

    float theta = acos(pow(dist(mt), (1.0f / (intersection.material.property.reflectance.s + 1.0f))));
    float phi = 2.0f * (float) M_PI * dist(mt);

    result.second = norm(changeFromBase(X, Y, Z, position) *
            hVector(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta)));

    result.first = getColor(intersection.material.property.reflectance.kdPhong, position) / (float) M_PI
                   + getColor(intersection.material.property.reflectance.ksPhong, position)
                     * (intersection.material.property.reflectance.s + 2.0f) / (2.0f * (float) M_PI)
                     * pow(abs(dot(n, result.second)), intersection.material.property.reflectance.s);

    return result;
}

void renderRegion(int j_ini, int j_end, int width, int height, int ppp, const Scene &scene, bool last, Image &image,
        Progress &progress) {
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
                HCoord direction = norm(getRay(scene.camera, ((float) i + dist(mt)) / (float) width,
                                               ((float) j + dist(mt)) /
                                               (float) height)); // should be a normalized ray
                HCoord position = scene.camera.origin;

                Color pathColor = C_BLACK;

                // Get color
                vector<pair<float, Color>> colors;

                bool path = true;
                while (path) {
                    // find nearest intersection
                    const Object *intersection = nullptr;
                    float dist = INFINITY;
                    for (const Object &object : scene.objects) {
                        float obj_dist = intersect(position, direction, object);
                        if (obj_dist > EPS && obj_dist < dist) {
                            intersection = &object;
                            dist = obj_dist;
                        }
                    }

                    if (intersection == nullptr) { // Any light founded
                        colors.clear(); // TODO: Muchos rayos se van de la escena
                        path = false;
                    } else {
                        HCoord origin = position;
                        position = position + direction * dist; // hit position
                        HCoord n = normal(intersection->geometry, position);

                        switch (intersection->material.type) {
                            case EMITTER: { // LIGHT
                                pathColor = getColor(intersection->material.property.texture, position);
                                for (auto c = colors.rbegin(); c != colors.rend(); ++c) {
                                    pathColor = pathColor * c->first + c->second * (1.0f - c->first);
                                }
                                path = false;
                                colors.clear();
                                break;
                            }
                            case REFLECTOR: {
                                uniform_real_distribution<float> zeroToOneDistribution(0.0f, 1.0f);
                                float randomZeroToOne, maxKd, maxKs, maxKdPhong, maxKsPhong, pr[3];

                                maxKd = getColor(intersection->material.property.reflectance.kd, position).max();
                                maxKs = getColor(intersection->material.property.reflectance.ks, position).max();
                                maxKdPhong = getColor(intersection->material.property.reflectance.kdPhong,
                                                      position).max();
                                maxKsPhong = getColor(intersection->material.property.reflectance.ksPhong,
                                                      position).max();

                                // Russian roulette
                                randomZeroToOne = zeroToOneDistribution(mt);
                                pr[0] = maxKd;
                                pr[1] = maxKs;
                                pr[2] = maxKdPhong + maxKsPhong;
                                if (pr[0] + pr[1] + pr[2] > 0.99f) {
                                    pr[1] = (0.99f / (pr[0] + pr[1] + pr[2]));
                                    pr[0] = maxKd * pr[1];
                                    pr[2] = pr[2] * pr[1];
                                    pr[1] = maxKs * pr[1];
                                }

                                pair<Color, HCoord> result;
                                if (randomZeroToOne < pr[0]) {
                                    // Perfect refraction case (delta BTDF)
                                    result = refraction(scene, origin, position, direction, n, *intersection);

                                    colors.emplace_back(result.first.max(), result.first);
                                } else if (randomZeroToOne < pr[0] + pr[1]) {
                                    // Perfect specular reflectance case (delta BRDF)
                                    result = reflection(position, -direction, n, *intersection);

                                    colors.emplace_back(result.first.max(), result.first);
                                } else if (randomZeroToOne < pr[0] + pr[1] + pr[2]) {
                                    // Perfect Phong case (Phong BRDF)
                                    result = phong(scene, position, -direction, n, *intersection, mt);

                                    colors.emplace_back(result.first.max(), result.first);
                                } else {
                                    // Path deaths
                                    colors.clear();
                                    path = false;
                                }
                                direction = result.second;

                                break;
                            }
                            default: {
                                exit(6);
                            }
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
    return render(width, height, ppp, scene, (int) thread::hardware_concurrency() + 2); // TODO: +2 para que trabajen más rato?
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
