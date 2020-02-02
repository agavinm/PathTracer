/******************************************************************************
 * @file    PhotonMapping.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 5: Photon mapping
 ******************************************************************************/

#include "Geometry.hpp"
#include "PhotonMapping.hpp"
#include "BRDF.hpp"
#include "Transform.hpp"

using namespace std;

Color PhotonMap::getColorFromMap(HCoord position, HCoord direction, const Object *object) const {
    const int ELEMENTS = 100;
    const int k = 10; // kernel

    Color color = C_BLACK;
    vector<const KDTree<Photon, 3>::Node *> list;
    float radius;
    // using find, if the tree don't exists an error is generated
    map.find(object)->second.find(position.as_vector(), ELEMENTS, list, radius);
    for (auto &node : list) {
        Photon photon = node->data();
        color = color +
                photon.color
                * getBRDF(PHONG_DIFFUSE, photon.direction, -direction, position, *object);
    }

    return color / ELEMENTS // average color
           / (M_PI * radius * radius) // color = total_light / area_circle(pi*r^2)
           / k; // kernel
}


void PhotonMap::addAll(vector<Photon> &other) {
    mtx.lock();
    for (Photon photon : other) {
        // using [] if the tree don't exist, it is initialized
        map[photon.object].store(photon.position.as_vector(), photon);
    }
    mtx.unlock();
}

void PhotonMap::markToRead() {
    mtx.lock();
    for (const auto& tree : map) {
        // tree.second.balance(); // for some reason this doesn't work
        map[tree.first].balance();
    }
    mtx.unlock();
}