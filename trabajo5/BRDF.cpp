/******************************************************************************
 * @file    BRDF.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#include "Transform.hpp"
#include "Render.hpp"
#include <cmath>
#include "BRDF.hpp"
#include "Random.hpp"

using namespace std;

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
 * @return
 */
HCoord refract(const HCoord &in, HCoord n, stack<const Object*> &refractionStack,
        float sceneRefractiveIndex, const Object* object) {
    // https://en.wikipedia.org/wiki/Snell%27s_law#Vector_form
    float c = -dot(n, in), refractionRatio;
    if (c < 0) {
        // the normal and the ray have the same direction
        c = -c;
        n = -n;
    }

    if (refractionStack.empty()) {
        // the ray comes from the outside
        refractionRatio = sceneRefractiveIndex / object->refractiveIndex;
        refractionStack.push(object);
    } else if (object != refractionStack.top()) {
        // the ray comes from another object
        refractionRatio = refractionStack.top()->refractiveIndex / object->refractiveIndex;
        refractionStack.push(object);
    } else {
        // the ray comes from the inside, the refraction index is the opposite
        refractionRatio = object->refractiveIndex / refractionStack.top()->refractiveIndex;
        refractionStack.pop();
    }

    float radicand = 1.0f - refractionRatio * refractionRatio * (1.0f - c * c);
    if (radicand < 0.0f) {
        // Only happens reflection because the sine of the angle of refraction is required to be greater than one.
        // https://en.wikipedia.org/wiki/Snell%27s_law#Total_internal_reflection_and_critical_angle
        return reflect(in, n);
    } else {
        return norm(in * refractionRatio + n * (refractionRatio * c - sqrt(radicand)));
    }
}

/**
 * Returns a new direction to follow (the 'bounce')
 * @param event which type of event to use
 * @param position position of bounce
 * @param direction incoming direction of bounce (towards position)
 * @param object where the bounce occurs
 * @return new direction
 */
HCoord getNewDirection(EVENT event, const HCoord &position, const HCoord &direction, const Object &object,
        stack<const Object*> &refractionStack, float sceneRefractiveIndex) {
    HCoord n = normal(object.geometry, position);
    switch (event) {
        case REFRACTION:
            return refract(direction, n, refractionStack, sceneRefractiveIndex, &object);
        case REFLECTION:
            return reflect(direction, n);
        case PHONG_DIFFUSE: {
            HCoord Z;
            HCoord Y;
            HCoord X;

            if (dot(direction, n) < 0) {
                // good side
                Z = n;
            } else {
                // opposite side
                Z = -n;
            }
            Y = norm(cross(Z, direction));
            X = norm(cross(Y, Z));

            float theta = (float) M_PI / 2.0f * random_zero_one();//acos(1 - random_zero_one());
            float phi = 2.0f * (float) M_PI * random_zero_one();

            return norm(changeFromBase(X, Y, Z, position) * hVector(cos(phi) * sin(theta), sin(phi) * sin(theta), cos(theta)));
        }
        case PHONG_SPECULAR: {
            HCoord ref = reflect(direction, n);

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

            float theta = acos(pow(random_zero_one(), (1.0f / (object.material.property.reflectance.s + 1.0f))));
            float phi = 2.0f * (float) M_PI * random_zero_one();

            return norm(changeFromBase(X, Y, Z, position) *
                        hVector(cos(phi) * sin(theta), cos(theta), sin(phi) * sin(theta)));
        }
        case DEAD:
            return V_ZERO;
    }
}

/**
 * The 'Russian roulette'
 * @param object of intersection
 * @param position of intersection
 * @return a random valid event
 */
EVENT getRandomEvent(const Object &object, const HCoord &position) {
    // Russian roulette

    // get percentages based on the max value of each color
    float maxKd = getColor(object.material.property.reflectance.kd, position).max();
    float maxKs = getColor(object.material.property.reflectance.ks, position).max();
    float maxKdPhong = getColor(object.material.property.reflectance.kdPhong, position).max();
    float maxKsPhong = getColor(object.material.property.reflectance.ksPhong, position).max();

    // cap to max value
    const float MAX = 0.99f;
    float sum = maxKd + maxKs + maxKdPhong + maxKsPhong;
    if (sum > MAX) {
        maxKd *= MAX / sum;
        maxKs *= MAX / sum;
        maxKdPhong *= MAX / sum;
        maxKsPhong *= MAX / sum;
    }

    float randomZeroToOne = random_zero_one();
    if ((randomZeroToOne -= maxKd) < 0) {
        // Perfect refraction case (delta BTDF)
        return REFRACTION;
    } else if ((randomZeroToOne -= maxKs) < 0) {
        // Perfect specular reflectance case (delta BRDF)
        return REFLECTION;
    } else if ((randomZeroToOne -= maxKdPhong) < 0) {
        // Perfect Phong case (Phong BRDF)
        return PHONG_DIFFUSE;
    } else if ((randomZeroToOne -= maxKsPhong) < 0) {
        // Perfect Phong case (Phong BRDF)
        return PHONG_SPECULAR;
    } else {
        // Path deaths
        return DEAD;
    }
}

/**
 * The Equation
 * @param event which event to compute
 * @param in input vector, towards the bounce
 * @param out output vector, away from the bounce
 * @param position where the bounce ocurred
 * @param object involved in the bounce
 * @return the factor (color) of the bounce
 */
Color getBRDF(EVENT event, const HCoord &in, const HCoord &out, const HCoord &position, const Object &object) {
    switch (event) {
        case REFRACTION: {
            return getColor(object.material.property.reflectance.kd, position);
        }
        case REFLECTION: {
            return getColor(object.material.property.reflectance.ks, position);
        }
        case PHONG_DIFFUSE: {
            return getColor(object.material.property.reflectance.kdPhong, position) / (float) M_PI;
        }
        case PHONG_SPECULAR: {
            return getColor(object.material.property.reflectance.ksPhong, position)
                   * (object.material.property.reflectance.s + 2.0f) / (2.0f * (float) M_PI)
                   * pow(abs(dot(in, out)), object.material.property.reflectance.s);
        }
        case DEAD:
            return C_BLACK;
    }
}
