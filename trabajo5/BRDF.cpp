//
// Created by abeln on 29/01/2020.
//

#include "FotonMapping.hpp"
#include "Transform.hpp"
#include "Progress.hpp"
#include "Render.hpp"
#include <iostream>
#include <random>
#include <cmath>
#include <cassert>
#include <thread>
#include "BRDF.hpp"
#include "Random.hpp"

/**
 * Reflects a vector from another, like a two-sides mirror
 * @param in input vector, like 'entering' a mirror
 * @param n normal of the mirror
 * @return output vector, like 'exiting' the mirror
 */
HCoord reflect(const HCoord &in, const HCoord &n) {
    return norm(in - n * dot(in, n) * 2.0f);
}

/**
 * Refracts a vector from another, like water
 * @param in input vector, like 'entering water'
 * @param n normal of water surface to air
 * @param refractionIndex the refraction index from water/air (aka normal_negative / normal_positive)
 * @return
 */
HCoord refract(const HCoord &in, HCoord n, float refractionIndex) {
    // https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
    float c = dot(-n, in);
    if (c < 0) {
        // the ray comes from the inside, the refraction index is the opposite
        refractionIndex = 1 / refractionIndex;
        c = -c;
        n = -n;
    }
    float radicand = 1.0f - refractionIndex * refractionIndex * (1.0f - c * c);
    if (radicand < 0.0f) {
        // Only happens reflection because the sine of the angle of refraction is required to be greater than one.
        // https://en.wikipedia.org/wiki/Snell%27s_law#Total_internal_reflection_and_critical_angle
        return reflect(in, n);
    } else {
        return norm(in * refractionIndex + n * (refractionIndex * c - sqrt(radicand)));
    }
}

pair<Color, HCoord> reflection(const HCoord &position, const HCoord &direction, const HCoord &n, const Object &intersection) {
    pair<Color, HCoord> result;

    result.first = getColor(intersection.material.property.reflectance.ks, position);

    result.second = reflect(direction, n);

    return result;
}


pair<Color, HCoord> refraction(const Scene &scene, const HCoord &origin, const HCoord &position, const HCoord &direction, HCoord n, const Object &intersection) {
    pair<Color, HCoord> result;

    result.first = getColor(intersection.material.property.reflectance.kd, position);

//    HCoord intermediate = hPoint((origin.x() + position.x()) / 2.0f,
//                                 (origin.y() + position.y()) / 2.0f, (origin.z() + position.z()) / 2.0f);
//    float n1 = scene.refractiveIndex, n2 = scene.refractiveIndex;
//    const Object *o = nullptr;
//    bool insideOf_o = false;
//    for (unsigned long objectIndex = 0; !insideOf_o && objectIndex < scene.objects.size(); objectIndex++) {
//        o = &scene.objects[objectIndex];
//        if (o->material.type == REFLECTOR && o->type == OBJECT_3D && isInside(intermediate, *o)) {
//            // Ray is inside an object
//            n1 = o->n;
//            insideOf_o = true;
//        }
//    }
//    if (insideOf_o) {
//        // Ray is inside an object
//        n = -n;
//        switch (o->geometry.type) {
//            case SPHERE: {
//                if (mod(position - o->geometry.data.sphere.center) == o->geometry.data.sphere.radius) {
//                    // Ray will come out of the sphere
//                    bool objectFound = false;
//                    for (unsigned long objectIndex = 0; !objectFound && objectIndex < scene.objects.size(); objectIndex++) {
//                        const Object *outside = &scene.objects[objectIndex];
//                        if (outside->material.type == REFLECTOR && outside->type == OBJECT_3D &&
//                            isInside(position, *outside) && outside != o) {
//                            // Ray will enter an outside object
//                            n2 = outside->n;
//                            objectFound = true;
//                        }
//                    }
//                } else {
//                    // Ray will enter another object
//                    n2 = intersection.n;
//                }
//
//                break;
//            }
//            default: {
//                exit(6);
//            }
//        }
//    } else {
//        // Ray will enter an object
//        n2 = intersection.n;
//    }

    result.second = refract(direction, n, scene.refractiveIndex / intersection.n);
    return result;
}

pair<Color, HCoord> phong(const Scene &scene, const HCoord &position, const HCoord &direction, const HCoord &n, const Object &intersection) {
    pair<Color, HCoord> result;

    HCoord ref = reflection(position, direction, n, intersection).second;

    HCoord Z;
    HCoord Y;
    HCoord X;

    if (abs(dot(direction, n)) < EPS) {
        Z = direction;
        Y = n;
        X = cross(Y, Z);
    } else {
        X = norm(cross(direction, ref));
        Y = ref;
        Z = cross(Y, X);
    }

    float theta = acos(pow(random_zero_one(), (1.0f / (intersection.material.property.reflectance.s + 1.0f))));
    float phi = 2.0f * (float) M_PI * random_zero_one();

    result.second = norm(changeFromBase(X, Y, Z, position) *
                         hVector(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta)));

    result.first = getColor(intersection.material.property.reflectance.kdPhong, position) / (float) M_PI
                   + getColor(intersection.material.property.reflectance.ksPhong, position)
                     * (intersection.material.property.reflectance.s + 2.0f) / (2.0f * (float) M_PI)
                     * pow(abs(dot(ref, result.second)), intersection.material.property.reflectance.s);

    return result;
}


EVENT getRandomEvent(const Object &object, const HCoord &position) {

    float maxKd = getColor(object.material.property.reflectance.kd, position).max();
    float maxKs = getColor(object.material.property.reflectance.ks, position).max();
    float maxPhong = getColor(object.material.property.reflectance.kdPhong, position).max()
                     + getColor(object.material.property.reflectance.ksPhong, position).max();

    // Russian roulette
    if (maxKd + maxKs + maxPhong > 0.99f) {
        float correction = 0.99f / (maxKd + maxKs + maxPhong);
        maxKd *= correction;
        maxKs *= correction;
        maxPhong *= correction;
    }

    float randomZeroToOne = random_zero_one();
    if ((randomZeroToOne -= maxKd) < 0) {
        // Perfect refraction case (delta BTDF)
        return REFRACTION;
    } else if ((randomZeroToOne -= maxKs) < 0) {
        // Perfect specular reflectance case (delta BRDF)
        return REFLECTION;
    } else if ((randomZeroToOne -= maxPhong) < 0) {
        // Perfect Phong case (Phong BRDF)
        return PHONG;
    } else {
        // Path deaths
        return DEAD;
    }
}

Color getBRDF(EVENT event, const HCoord &in, const HCoord &out, const HCoord &position, const Object &object) {

    const float CONSIDER_EQUALS = 0.05;

    switch (event) {
        case REFRACTION: {
            HCoord n = normal(object.geometry, position);
            HCoord refraction = refract(in, n, VACUUM_REFRACTIVE_INDEX / WATER_REFRACTIVE_INDEX);
            // TODO: this

            return dot(refraction, out) >= 1 - CONSIDER_EQUALS // if practically the same vector
                   ? getColor(object.material.property.reflectance.kd, position) // valid
                   : C_BLACK; // otherwise no
        }
        case REFLECTION: {
            HCoord n = normal(object.geometry, position);
            HCoord reflection = reflect(in, n);

            return dot(reflection, out) >= 1 - CONSIDER_EQUALS // if practically the opposite vector
                   ? getColor(object.material.property.reflectance.ks, position) // valid
                   : C_BLACK; // otherwise no
        }
        case PHONG:
            if (dot(in, out) >= 0) {
                // phong is valid only on the positive semisphere
                return C_BLACK;
            }
            return getColor(object.material.property.reflectance.kdPhong, position) / (float) M_PI
                   + getColor(object.material.property.reflectance.ksPhong, position)
                     * (object.material.property.reflectance.s + 2.0f) / (2.0f * (float) M_PI)
                     * pow(abs(dot(in, out)), object.material.property.reflectance.s);
        case DEAD:
            return C_BLACK;
    }
}
