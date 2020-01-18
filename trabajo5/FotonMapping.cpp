//
// Created by abeln on 18/01/2020.
//

#include "Geometry.hpp"
#include "FotonMapping.hpp"

using namespace std;


Color FotonMap::getColorFromMap(HCoord position, HCoord direction, float dist) const {
    int fotons = 0;
    Color color = C_BLACK;
    for(Foton foton : map){
        // only fotons near the element
        // TODO: reduce 'sphere' size and return correct color
        if(mod(foton.position-position) < 1 && dot(direction, foton.direction) >= 0){
            fotons++;
            color = color + foton.color * dot(direction, foton.direction) / (foton.dist + dist);
        }
    }
    return color / fotons;
}


void FotonMap::addFoton(Foton foton) {
    map.push_back(foton);
}

void FotonMap::addAll(FotonMap &other) {
    mtx.lock();
    for(Foton foton : other.map){
        map.push_back(foton); // because insert uses copy, and we don't have copy
    }
    mtx.unlock();
}
