//
// Created by abeln on 04/11/2019.
//

#include "Geometric.hpp"
#include "Scenes.hpp"

using namespace std;

#define SCENE 1

//////////////////////////////////////////////////////

#if SCENE == 1

std::vector<Object> getObjects() {
    return vector<Object>{
            {
                    Sphere(hPoint(10, 0, 0), 1),
                    Emitter(1, 0, 0)
            },
            {
                    Sphere(hPoint(5, 2, 0), 1),
                    Emitter(0, 1, 0)
            },
    };
}

Camera getCamera(float ratio) {
    return createCamera(
            hPoint(0, 0, 0),
            hVector(1, 0, 0),
            hVector(0, 0, 1),
            ratio
    );
}

#endif