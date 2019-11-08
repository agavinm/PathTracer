//
// Created by abeln on 04/11/2019.
//

#include <cmath>
#include "Geometric.hpp"
#include "Scenes.hpp"

using namespace std;

#define SCENE 3

//////////////////////////////////////////////////////


vector<Object> getObjects() {
    vector<Object> objects;
#if SCENE == 1
    objects.push_back({Sphere(hPoint(10, 0, 0), 1), Emitter({1, 0, 0})});
    objects.push_back({Sphere(hPoint(5, 5, 0), 1), Emitter({0, 1, 0})});
    objects.push_back({Plane(hVector(0, 0, 1), 10), Emitter({0, 0, 1})});
#elif SCENE == 2
    int n = 6;
    int m = 10;
    for (int i = 0; i < n * m; ++i) {
        float ang = (float) i / n * M_PI * 2;
        objects.push_back({Sphere(hPoint(-(n*m)/2 + i, 3 * sin(ang), 3 * cos(ang)), 1), Emitter({abs(cos(ang)), abs(sin(ang)), (float) i / (n * m)})});
    }
#elif SCENE == 3
    objects.push_back({Sphere(hPoint(3, 0, 0), 1),  Emitter({1, 0, 0})});
    objects.push_back({Sphere(hPoint(0, 3, 0), 1),   Emitter({0, 1, 0})});
    objects.push_back({Sphere(hPoint(0, 0, 3), 1), Emitter({0, 0, 1})});
#endif
    return objects;
}


Camera getCamera(float ratio) {
#if SCENE == 3
    return createCamera(
            hPoint(-5, 0, 0),
            hVector(1, 0, 0),
            hVector(0, 0, 1),
            ratio
    );
#else
    return createCamera(
            hPoint(0, 0, 0),
            hVector(1, 0, 0),
            hVector(0, 0, 1),
            ratio
    );
#endif
}
