//
// Created by abeln on 18/01/2020.
//

#include "Geometry.hpp"
#include "FotonMapping.hpp"

using namespace std;


Color FotonMap::getColorFromMap(HCoord position, HCoord direction, float distance, const Object *object) const {
    const int ELEMENTS = 100;

    Color color = C_BLACK;
    vector<const KDTree<Foton, 3>::Node *> list;
    float radius;
    tree.find(position.as_vector(), ELEMENTS, list, radius);
    for(auto &node : list){
        Foton foton = node->data();
        // only fotons near the element and from the same object
        if(dot(direction, foton.direction) >= 0 && object == foton.object){
            color = color + foton.color * dot(direction, foton.direction) / (foton.dist + distance);
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
    mtx.lock();
    tree.balance();
    mtx.unlock();
}