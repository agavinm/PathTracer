/******************************************************************************
 * @file    Scene.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#include <cmath>
#include <map>
#include <fstream>
#include <iostream>
#include "Scene.hpp"
#include "Texture.hpp"
#include "Random.hpp"
#include "Transform.hpp"
#include "Color.hpp"

using namespace std;

//////////////////////////////////////////////////////
/// black magic to load scenes dynamically
//////////////////////////////////////////////////////

typedef Scene (*scene_function)(float);

map<string, scene_function> *scenes_map;

void __attribute__((constructor)) __initScenes__() {
    scenes_map = new map<string, scene_function>();
}

#define defineScene(name)                                                           \
    Scene name##Scene(float ratio);                                                 \
    void __attribute__((constructor)) __defineScene_##name () {                     \
        (*scenes_map).insert( pair<string, scene_function>( #name, name##Scene ) ); \
    }                                                                               \
    Scene name##Scene(float ratio)

//////////////////////////////////////////////////////

Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio) {
    return {
            .origin=origin,
            .front=front,
            .left=norm(cross(up, front)) * mod(up) * ratio, // left is ortogonal to up and front, with the size of up*ratio
            .up=up
    };
}

HCoord getRay(const Camera &camera, float i, float j) {
    return camera.front + camera.left * (1 - 2 * i) + camera.up * (1 - 2 * j);
}

/**
 * The default box (5 planes)
 * @param dist distance from the center to the planes
 * @param topEmitter if true, the top plane will be emitter, if not will be diffuse
 * @param objects where to add the planes
 */
void addDefaultBox(float dist, bool topEmitter, vector<Object> &objects) {
    objects.push_back(createObject(
            Plane(hVector(0, 0, -1), dist),
            (topEmitter ? Emitter : Diffuse)(colored(C_WHITE))
    )); // UP
    objects.push_back(createObject(
            Plane(hVector(-1, 0, 0), dist),
            Diffuse(colored(C_GREY))
    )); // FRONT
    objects.push_back(createObject(
            Plane(hVector(0, 1, 0), dist),
            Diffuse(colored(C_GREEN))
    )); // RIGHT
    objects.push_back(createObject(
            Plane(hVector(0, -1, 0), dist),
            Diffuse(colored(C_RED))
    )); // LEFT
    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), dist),
            Diffuse(colored(C_GREY))
    )); // DOWN
}

defineScene(default) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(2.5f, 0, -4)));

    // BOX:
    addDefaultBox(5, true, objects);

    // SPHERES:
    objects.push_back(createObject(
            Sphere(hPoint(3, 2.5f, -2.5f), 1.5f),
            Phong(colored(C_BLUE), colored(C_GREEN), 10),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 2),
            Delta(colored(C_BLUE), colored(C_YELLOW)), // Refracts blue and reflects yellow
            WATER_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

defineScene(contest) {
    Camera camera = createCamera(hPoint(1, 0, 2), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // CAR
    vector<LightPoint> lightPoints;
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(9, -1, 1)));
    objects.push_back(createObject(
            Sphere(hPoint(9.5f, 0.2f, 1.25f), 0.2f),
            Emitter(colored(C_YELLOW))
    ));
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(7, 2, 1)));
    objects.push_back(createObject(
            Sphere(hPoint(7.5f, 2.95f, 1), 0.23f),
            Emitter(colored(C_YELLOW))
    ));
    loadPly("ply/car.ply", objects, false);
    // END_CAR

    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), 1),
            Diffuse(colored(C_GREEN))
    )); // GROUND

    // CAUSTICS
    objects.push_back(createObject(
            Cuadric(1, 1, -0.2f, 0, 0, 0, -15, 5, 0, 62),
            Delta(colored(C_WHITE), colored({0.6, 0.6, 1})),
            WATER_REFRACTIVE_INDEX
    ));
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(8, -2, 1)));
    objects.push_back(createObject(
            Sphere(hPoint(7.5f, -2.5f, 0), 1),
            Refractor(colored(C_YELLOW)),
            AMBER_REFRACTIVE_INDEX
    ));
    // END_CAUSTICS

    // MIRROR
    objects.push_back(createObject(
            Triangle(hPoint(10, -7.5f, -1), hPoint(6, 0, 6), hPoint(3, -3.5f, 1)),
            Specular(colored(C_WHITE))
    ));
    // END_MIRROR

    // RING
    int R = 100;
    for (int i = 0; i < R; ++i) {
        float angle = 2 * M_PI * i / R;
        objects.push_back(createObject(
                Sphere(hPoint(7.5f + 1.5f * sin(angle), -2.5f + 1.5f * cos(angle), 3 - 1.5f * sin(angle)), 0.5f),
                Phong(colored(C_YELLOW), colored(C_YELLOW), 10)
        ));
    }
    // END_RING

    // Stars
    /*for (int i = 0; i < 100; i++) {
        int y = random_zero_n(250) - 125, z = random_zero_n(83);
        lightPoints.push_back(createLightPoint(C_WHITE, hPoint(45, y, z + 5)));
        objects.push_back(createObject(
                Sphere(hPoint(50, y, z), random_zero_one()),
                Diffuse(colored(C_WHITE * random_zero_one()))
        ));
    }*/

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = AIR_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

defineScene(caustics) {
    float radius = 15;
    Camera camera = createCamera(hPoint(-radius * 2 + EPS, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * 1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * 1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * -1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * -1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * 1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * 1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * -1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * -1.5f, radius * -1.5f)));
    lightPoints.push_back(createLightPoint(C_WHITE, P_ZERO));

    // BOX:
    objects.push_back(createObject(
            Plane(hVector(0, 0, -1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // UP
    objects.push_back(createObject(
            Plane(hVector(-1, 0, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // FRONT
    objects.push_back(createObject(
            Plane(hVector(0, 1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // RIGHT
    objects.push_back(createObject(
            Plane(hVector(0, -1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // LEFT
    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // DOWN
    objects.push_back(createObject(
            Plane(hVector(1, 0, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // BACK

//    objects.push_back(createObject(
//            Sphere(
//                    P_ZERO,
//                    radius * 2
//            ),
//            Reflector(
//                    colored(C_BLACK),
//                    colored(C_WHITE),
//                    1,
//                    colored(C_WHITE),
//                    colored(C_BLACK)
//            ),
//            AIR_REFRACTIVE_INDEX//random_float(VACUUM_REFRACTIVE_INDEX, AIR_REFRACTIVE_INDEX)
//    ));


    // SPHERES:
    int n = 100;
    for (int i = 0; i < n; ++i) {

        double theta = 2 * M_PI * random_zero_one();
        double phi = acos(1 - 2 * random_zero_one());
        HCoord direction = hVector(sin(phi) * cos(theta), sin(phi) * sin(theta), cos(phi));

        float local_radius = random_float(radius / 8, radius / 4);

        objects.push_back(createObject(
                Sphere(
                        P_ZERO + direction * (radius - local_radius),
                        local_radius
                ),
                Delta(
                        colored(hsv(random_float(0, 360), 1.0, 1.0)),
                        colored(hsv(random_float(0, 360), 1.0, 1.0))
                ),
                random_float(1.01, 1.1)
        ));
    }

//    // CUADRICS
//    for (int i = 0; i < 1; ++i) {
//
//        objects.push_back(createObject(
//                Cuadric(
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10),
//                        random_float(-10, 10)
//                ),
//                Delta(
//                        colored(hsv(random_float(0, 360), 1.0, 1.0)),
//                        colored(hsv(random_float(0, 360), 1.0, 1.0))
//                ),
//                random_float(1.001, 1.01)
//        ));
//    }

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f,
//            .clampCorrection = 25.0f
    };

}

defineScene(layers) {
    float radius = 15;
    Camera camera = createCamera(hPoint(-radius * 4 + EPS, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * 1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * 1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * -1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * 1.5f, radius * -1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * 1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * 1.5f, radius * -1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * -1.5f, radius * 1.5f)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(radius * -1.5f, radius * -1.5f, radius * -1.5f)));
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(0, radius, 0)));

    // BOX:
    objects.push_back(createObject(
            Plane(hVector(0, 0, -1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // UP
    objects.push_back(createObject(
            Plane(hVector(-1, 0, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // FRONT
    objects.push_back(createObject(
            Plane(hVector(0, 1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // RIGHT
    objects.push_back(createObject(
            Plane(hVector(0, -1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // LEFT
    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // DOWN
    objects.push_back(createObject(
            Plane(hVector(1, 0, 0), radius * 4),
            Diffuse(colored(C_WHITE))
    )); // BACK

    objects.push_back(createObject(
            Sphere(hPoint(0, 0, -radius), radius),
            Refractor(colored(C_WHITE)),
            AIR_REFRACTIVE_INDEX//random_float(VACUUM_REFRACTIVE_INDEX, AIR_REFRACTIVE_INDEX)
    ));


    // Planes:
    int N = 3;
    for (int i = 0; i < N; ++i) {

        objects.push_back(createObject(
                Plane(V_AZ, radius * 2 * (float) (i + 1) / (float) (N + 1)),
                Delta(
                        colored(C_WHITE),
                        colored(C_WHITE)
                ),
                1.01//random_float(1.001, 1.01)
        ));
    }

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f,
//            .clampCorrection = 25.0f
    };
}

defineScene(rainbow) {
    float radius = 15;
    Camera camera = createCamera(hPoint(-radius + EPS, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
    int lights = 8;
    for (int i = 0; i < lights; ++i) {
        float angle = (float) i / (float) lights;
        lightPoints.push_back(createLightPoint(
                hsv(angle * 360, 1.0, 1.0),
                hPoint(0, cos(angle * 2 * M_PI) * radius, sin(angle * 2 * M_PI) * radius)
        ));
    }

    // BOX:
    objects.push_back(createObject(
            Plane(hVector(0, 0, -1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // UP
    objects.push_back(createObject(
            Plane(hVector(-1, 0, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // FRONT
    objects.push_back(createObject(
            Plane(hVector(0, 1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // RIGHT
    objects.push_back(createObject(
            Plane(hVector(0, -1, 0), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // LEFT
    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), radius * 2),
            Diffuse(colored(C_WHITE))
    )); // DOWN
    objects.push_back(createObject(
            Plane(hVector(1, 0, 0), radius),
            Emitter(colored(C_BLACK))
    )); // BACK


    objects.push_back(createObject(
            Sphere(hPoint(radius, 0, 0), radius / 2),
            Refractor(colored(C_WHITE)),
            1.2
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f,
//            .clampCorrection = 25.0f
    };
}

defineScene(noEmitters) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(2.5f, 0, -3)));
    lightPoints.push_back(createLightPoint({1, 0.5, 0}, hPoint(2.5f, 2.5, 4.5)));

    // BOX:
    addDefaultBox(5, false, objects);

    // water
    objects.push_back(createObject(
            Plane(hVector(0, 0, 1), 3.5),
            Delta(colored(C_WHITE), colored({0.6, 0.6, 1})),
            WATER_REFRACTIVE_INDEX
    ));

    // SPHERES:
    objects.push_back(createObject(
            Sphere(hPoint(3, 2.5f, -2.5f), 1.5f),
//            Phong(colored(C_BLUE), colored(C_GREEN), 10),
            Specular(colored(C_WHITE)),
            VACUUM_REFRACTIVE_INDEX
    ));

    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 2),
//            Delta(colored(C_BLUE), colored(C_YELLOW)), // Refracts blue and reflects yellow
            Refractor(colored(C_WHITE)),
            WATER_REFRACTIVE_INDEX
    ));

    objects.push_back(createObject(
            Sphere(hPoint(3, -2.5f, 2.5f), 1.5f),
//            Phong(colored(C_BLUE), colored(C_GREEN), 10),
            Phong(colored(C_WHITE), colored(C_WHITE), 2),
            VACUUM_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f,
            .clampCorrection = 25.0f
    };
}

defineScene(specular) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // BOX:
    addDefaultBox(5, true, objects);

    // MIRROR
    objects.push_back(createObject(
            Triangle(hPoint(5, 0, 2), hPoint(5, 2, -2), hPoint(5, -2, -2)),
            Specular(colored(C_WHITE))
    )); // FRONT

    // SPHERES:
    objects.push_back(createObject(
            Sphere(hPoint(3, 4, -4), 1),
            Phong(colored(C_BLUE), colored(C_GREEN), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 2),
            Specular(colored(C_WHITE)),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(2, 3, 3), 1.5f),
            Specular(colored(C_WHITE)),
            VACUUM_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

defineScene(refraction) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // BOX:
    addDefaultBox(5, true, objects);

    // SPHERES:
    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 1),
            Phong(colored(C_GREEN * 0.5f + C_YELLOW * 0.5f), colored(C_GREEN * 0.5f + C_YELLOW * 0.5f), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 2),
            Refractor(colored(C_CYAN)),
            LIQUID_HELIUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, 2.5f, -2), 1.5f),
            Phong(colored(C_BLUE * 0.5f + C_GREEN * 0.5f), colored(C_BLUE * 0.5f + C_GREEN * 0.5f), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, 3, -3), 2),
            Refractor(colored(C_YELLOW)),
            AMBER_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

defineScene(circle) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // BOX:
    addDefaultBox(5, true, objects);

    // SPHERES:
    objects.push_back(createObject(
            Sphere(hPoint(3, 2.5f, -2.5f), 1.5f),
            Phong(colored(C_BLUE), colored(C_GREEN), 10),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(createObject(
            Sphere(hPoint(3, -3, -3), 2),
            Delta(colored(C_BLUE), colored(C_YELLOW)), // Refracts blue and reflects yellow
            WATER_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 5.0f
    };
}

defineScene(donut) {
    Camera camera = createCamera(
            hPoint(-9, 9, 5),
            norm(hVector(1, -1, -1)),
            norm(hVector(1, -1, 1)),
            ratio);

//    Camera camera = createCamera(hPoint(-10, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(-9, -9, 9)));
//    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(-2.5f, 5, 0)));
//    lightPoints.push_back(createLightPoint(C_YELLOW, hPoint(-5, -5, 0)));

    // BOX:
    addDefaultBox(20, false, objects);

//    objects.push_back(createObject(
//            Plane(hVector(0,0,1), 6),
//            Diffuse(colored(C_WHITE))
//    ));

    int R = 100;
    for (int i = 0; i < R; ++i) {
        float angle = 2 * M_PI * i / R;
        objects.push_back(createObject(
                Sphere(hPoint(0, 3 * cos(angle), 3 * sin(angle)), 1),
                Diffuse(colored(C_YELLOW))
        ));
    }


//    objects.push_back(createObject(
//            Sphere(hPoint(0, 0,0), 2),
//            Diffuse(colored(C_WHITE))
//    ));

//    loadPly("ply/figure.ply", objects, false);

//    loadPly("ply/monkey.ply", objects, false);

//    loadPly("ply/donut500.ply", objects, false);

    objects.push_back(createObject(
            Sphere(hPoint(0, 0, 0), 5),
            Refractor(colored(C_WHITE)),
            1.1
    ));


    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f
    };
}

defineScene(dna) {
    Camera camera = createCamera(hPoint(-200, 0, 0), V_AX, V_AZ, ratio);
    vector<Object> objects;

    // BOX:
    addDefaultBox(100, true, objects);

    for (int i = 0; i <= 100; i += 1) {
        objects.push_back(createObject(
                Sphere(hPoint(-50 * cos(i / 5.), -50 * sin(i / 5.), i * 2 - 100), 5),
                Diffuse(colored(C_PURPLE)),
                VACUUM_REFRACTIVE_INDEX
        ));
    }

    objects.push_back(createObject(
            Cuadric(1, 1, 0, 0, 0, 0, 0, 0, 0, -40 * 40),
            Diffuse(colored(C_WHITE))
    ));
    objects.push_back(createObject(
            Cuadric(0.1, -0.1, 0, 0, 0, 0, 0, 0, 4, 500),
            Diffuse(colored(C_CYAN))
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.5f
    };
}

defineScene(report) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);
    vector<Object> objects;
    vector<LightPoint> lightPoints;

    // LIGHT:
    lightPoints.push_back(createLightPoint(C_WHITE, hPoint(2.5f, 0, 4.5)));
//    lightPoints.push_back(createLightPoint(C_PURPLE, hPoint(4, 4, -4)));
//    lightPoints.push_back(createLightPoint(C_GREEN, hPoint(1, -3, -3)));
//    objects.push_back(createObject(
//            Sphere(hPoint(2.5f,0,4.5f), 0.1f),
//            Emitter(colored(C_WHITE))
//    ));
//    objects.push_back(createObject(
//            Sphere(hPoint(0,0,100), 95.05),
//            Emitter(colored(C_WHITE))
//    ));

    // BOX:
    addDefaultBox(5, false, objects);

//    objects.push_back(createObject(
//            Plane(hVector(1,0,0), -5),
//            Diffuse(colored(C_BLACK))
//    ));

//    for(int i=0;i<=5;++i) {
//        objects.push_back(createObject(
//                Sphere(hPoint(3, (i-2.5)*1.5, 1), 0.65),
//                Phong(colored(C_BLACK), colored(C_WHITE), i*2),
//                1.01
//        ));
//    }
//
//    objects.push_back(createObject(
//            Cuadric(1, 1, 0, 0, 0, 0, 0, 0, 0, -40 * 40),
//            Diffuse(colored(C_PURPLE))
//    ));
    objects.push_back(createObject(
            Sphere(hPoint(2.5f, 3, -1), 1.5f),
            Refractor(colored(C_WHITE)),
            1.1
    ));
    objects.push_back(createObject(
            Sphere(hPoint(2.5f, -3, -1), 1.5f),
            Specular(colored(C_WHITE)),
            1.1
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 3.0f,
//            .clampCorrection = 0.3f
    };
}

Scene onlyPlyScene(const string &filename, float ratio) {
    Camera camera = createCamera(P_ZERO, V_AX, V_AZ, ratio);

    vector<Object> objects;

    loadPly(filename, objects, true);

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 1.0f
    };
}

void loadPly(const string &filename, vector<Object> &objects, bool isEmmiter) {
    ifstream file(filename);
    if (!file.is_open()) {
        // can't open, exit
        cerr << "The file " << filename << " can't be opened to read." << endl;
        exit(1);
    }

    bool header = true, vertex = false, face = false, color = false;
    string line, field;
    float x, y, z;
    int numVertices = 0, numFaces = 0, vertex1, vertex2, vertex3;
    unsigned char r, g, b;
    vector<pair<HCoord, Color>> vertices;
    vector<Object> triangles;
    float xMin = INFINITY, xMax = -INFINITY, yMin = INFINITY, yMax = -INFINITY, zMin = INFINITY, zMax = -INFINITY;

    while (getline(file, line)) {
        if (line.size() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        if (line.find("comment") != 0 && header) {
            if (line.find("element vertex ") == 0) {
                numVertices = stoi(line.substr(15));
                vertices.reserve(numVertices);
            } else if (line.find("element face ") == 0) {
                numFaces = stoi(line.substr(13));
            } else if (line == "end_header") {
                header = false;
                vertex = true;
            }
        } else if (line.find("comment") != 0 && vertex) {
            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            x = stof(field);

            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            y = stof(field);

            if (line.find(' ') != string::npos) { // Vertex color
                color = true;

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                z = stof(field);

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                r = stoi(field);

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                g = stoi(field);

                if (line.find(' ') != string::npos) {
                    field = line.substr(0, line.find(' '));
                    line.erase(0, field.length() + 1);
                } else {
                    field = line;
                }
                b = stoi(field);

                vertices.emplace_back(hPoint(x, y, z), rgb(r, g, b));
            } else { // Not vertex color
                z = stof(line);
                vertices.emplace_back(hPoint(x, y, z), C_WHITE);
            }

            xMin = min(xMin, x);
            xMax = max(xMax, x);
            yMin = min(yMin, y);
            yMax = max(yMax, y);
            zMin = min(zMin, z);
            zMax = max(zMax, z);

            if (vertices.size() == numVertices) {
                vertex = false;
                face = true;
            }
        } else if (line.find("comment") != 0 && face) {
            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            if (stoi(field) != 3) {
                cerr << filename << " file must be a triangular ply file" << endl;
                exit(1);
            }

            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            vertex1 = stoi(field);

            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            vertex2 = stoi(field);

            vertex3 = stoi(line);

            Color col[3] = {vertices[vertex1].second,
                            vertices[vertex2].second, vertices[vertex3].second};

            HCoord vert[3] = {vertices[vertex1].first, vertices[vertex2].first,
                              vertices[vertex3].first};

            if (isEmmiter) {
                triangles.push_back(createObject(
                        Triangle(vertices[vertex1].first, vertices[vertex2].first, vertices[vertex3].first),
                        color ? Emitter(colored(vertexColorDistanceWeightingSquare(col, vert)))
                              : Emitter(colored(C_WHITE))
                ));
            } else {
                triangles.push_back(createObject(
                        Triangle(vertices[vertex1].first, vertices[vertex2].first, vertices[vertex3].first),
                        color ? Diffuse(colored(vertexColorDistanceWeightingSquare(col, vert)))
                              : Diffuse(colored(C_WHITE))
                ));
            }

            numFaces--;
        }
    }

    if (!face || numFaces != 0 || triangles.empty()) {
        cerr << filename << " file must be a triangular ply file" << endl;
        exit(2);
    }

    objects.push_back(createTRIANGULAR_PLY(
            Sphere(hPoint((xMin + xMax) / 2.0f, (yMin + yMax) / 2.0f, (zMin + zMax) / 2.0f),
                   mod(hPoint(xMax, yMax, zMax) - hPoint(xMin, yMin, zMin))),
            triangles
    ));
}


Scene createScene(const string &scene, float ratio) {
    if ((*scenes_map).count(scene)) {
        return (*scenes_map)[scene](ratio);
    } else {
        return onlyPlyScene(scene, ratio);
    }
}

void printScenes() {
    cout << "Available scenes: ";
    for (const auto &scene : *scenes_map) {
        cout << scene.first << ", ";
    }
    cout << endl;
}