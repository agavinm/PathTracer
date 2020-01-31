//
// Created by abeln on 18/01/2020.
//

#include <assert.h>
#include "Geometry.hpp"
#include "FotonMapping.hpp"
#include "BRDF.hpp"
#include "Transform.hpp"

using namespace std;

Color FotonMap::getColorFromMap(HCoord position, HCoord direction, const Object *object) const {
    const int ELEMENTS = 100;

    Color color = C_BLACK;
    vector<const KDTree<Foton, 3>::Node *> list;
    float radius;
    // using find, if the tree don't exists an error is generated
    map.find(object)->second.find(position.as_vector(), ELEMENTS, list, radius);
    for (auto &node : list) {
        Foton foton = node->data();
        color = color +
                foton.color
                * getBRDF(PHONG_DIFFUSE, direction, -foton.direction, position, *object);
    }

    return color / M_PI / radius / radius // color = total_light / area_sphere(pi*r^2)
    / 5000; // magic number by testing
}


void FotonMap::addAll(vector<Foton> &other) {
    mtx.lock();
    for (Foton foton : other) {
        // using [] if the tree don't exist, it is initialized
        map[foton.object].store(foton.position.as_vector(), foton);
    }
    mtx.unlock();
}

void FotonMap::markToRead() {
    mtx.lock();
    for (auto tree : map) {
        // tree.second.balance(); // for some reason this doesn't work
        map[tree.first].balance();
    }
    mtx.unlock();
}