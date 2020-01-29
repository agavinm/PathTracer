//
// Created by abeln on 18/01/2020.
//

#include "Geometry.hpp"
#include "FotonMapping.hpp"

using namespace std;

Color FotonMap::getColorFromMap(HCoord position, HCoord direction, float distance, const Object *object) const {
    const int ELEMENTS = 500;

    Color color = C_BLACK;
    vector<const KDTree<Foton, 3>::Node *> list;
    float radius;
    // using find, if the tree don't exists an error is generated
    map.find(object)->second.find(position.as_vector(), ELEMENTS, list, radius);
    for (auto &node : list) {
        Foton foton = node->data();
        // only fotons that will be reflected
        // TODO: change this from the ugly diffuse (dot) to a valid from the object
        if (dot(direction, foton.direction) >= 0) {
            color = color + foton.color * dot(direction, foton.direction) / (foton.dist + distance);
        }
    }
    // color = total_light / area_sphere(pi*r^2)
    return color / M_PI / radius / radius;
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
    for(auto tree : map){
        // tree.second.balance(); // for some reason this doesn't work
        map[tree.first].balance();
    }
    mtx.unlock();
}