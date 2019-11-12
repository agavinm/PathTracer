/******************************************************************************
 * @file    Scenes.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include "Scenes.hpp"

using namespace std;

//////////////////////////////////////////////////////


vector<Object> getObjects(const string &scene) {
    vector<Object> objects;
    if (scene == "test") {
        objects.push_back({Sphere(hPoint(10, 0, 0), 1), Texturer(sin2D(C_RED))});
        objects.push_back({Sphere(hPoint(5, 5, 0), 1), Texturer(sin22D(C_GREEN))});
        objects.push_back({Plane(V_AZ, 10), Emitter({0, 0, 1})});
        objects.push_back({Circle(hPoint(5, -5, 0), -V_AX, V_AZ * 2), Texturer(sinCos2D(C_WHITE))});
        objects.push_back({Triangle(hPoint(10, 0, 0), hPoint(5, 5, 0), hPoint(5, 0, -5)), Emitter(C_PURPLE)});
    }

    if (scene == "spiral") {
        int n = 6;
        int m = 10;
        for (int i = 0; i < n * m; ++i) {
            float ang = (float) i / n * M_PI * 2;
            objects.push_back({Sphere(hPoint(-(n * m) / 2 + i, 3 * sin(ang), 3 * cos(ang)), 1), Emitter({abs(cos(ang)), abs(sin(ang)), (float) i / (n * m)})});
        }
    }

    if (scene == "XYZ") {
        objects.push_back({Sphere(hPoint(3, 0, 0), 1), Emitter(C_RED)});
        objects.push_back({Sphere(hPoint(0, 3, 0), 1), Emitter(C_GREEN)});
        objects.push_back({Sphere(hPoint(0, 0, 3), 1), Emitter(C_BLUE)});
    }
    return objects;
}


Camera getCamera(float ratio) {
    return createCamera(
            P_ZERO,
            V_AX,
            V_AZ,
            ratio
    );
}
