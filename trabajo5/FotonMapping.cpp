//
// Created by abeln on 18/01/2020.
//

#include "Geometry.hpp"
#include "FotonMapping.hpp"

using namespace std;


Color FotonMap::getColorFromMap(HCoord position, HCoord direction, float dist) const {
    const float SPHERE_SIZE = 0.01;

    int fotons = 0;
    Color color = C_BLACK;
    list<const KDTree<Foton, 3>::Node *> list;
    tree.find(position.as_vector(), SPHERE_SIZE, &list);
    for(auto &node : list){
        Foton foton = node->data();
        // only fotons near the element
        if(mod(foton.position-position) <= SPHERE_SIZE && dot(direction, foton.direction) >= 0){
            fotons++;
            color = color + foton.color * dot(direction, foton.direction) / (foton.dist + dist);
        }
    }
    // TODO: return correct color
    return fotons == 0 ? color : color / fotons;
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