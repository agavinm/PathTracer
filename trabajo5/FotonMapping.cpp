//
// Created by abeln on 18/01/2020.
//

#include "Geometry.hpp"
#include "FotonMapping.hpp"

using namespace std;


Color FotonMap::getColorFromMap(HCoord position, HCoord direction, float dist) const {
    const int ELEMENTS = 500;

    Color color = C_BLACK;
    vector<const KDTree<Foton, 3>::Node *> list;
    float radius;
    tree.find(position.as_vector(), ELEMENTS, list, radius);
    for(auto &node : list){
        Foton foton = node->data();
        // only fotons near the element
        if(dot(direction, foton.direction) >= 0){
            color = color + foton.color * dot(direction, foton.direction) / (foton.dist + dist);
        }
    }
    // color = total_light / area_sphere(pi*r^2)
    return color / M_PI / radius / radius;
}


void FotonMap::addAll(vector<Foton> &other) {
    mtx.lock();
    for(Foton foton : other){
        tree.store(foton.position.as_vector(), foton);
    }
    mtx.unlock();
}

void FotonMap::markToRead() {
    tree.balance();
}