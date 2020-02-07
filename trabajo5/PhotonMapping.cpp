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

//#define SEPARATE_BY_OBJECT

#ifdef SEPARATE_BY_OBJECT
#define key(k) k
#else
#define key(k) 0
#endif

Color PhotonMap::getColorFromMap(HCoord position, HCoord direction, const Object *object) const {
    const int ELEMENTS = 5000;
    float k = totalNumberOfPhotons / 30000.0f; // kernel

    Color color = C_BLACK;
    vector<const KDTree<Photon, 3>::Node *> list;
    float radius;
    // using find, if the tree doesn't exist an error is generated
    map.find(key(object))->second.find(position.as_vector(), ELEMENTS, list, radius);
    for (auto &node : list) {
        Photon photon = node->data();
        color = color +
                photon.color
                * getBRDF(PHONG_DIFFUSE, photon.direction, -direction, position, *object)
                / mod(photon.position - position);
    }

    return color / ELEMENTS // average color
           / (M_PI * radius * radius) // color = total_light / area_circle(pi*r^2)
           / k; // kernel
}


void PhotonMap::addAll(vector<Photon> &other) {
    mtx.lock();
    for (Photon photon : other) {
        // using [] if the tree doesn't exist, it is initialized
        map[key(photon.object)].store(photon.position.as_vector(), photon);
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

PhotonMap::PhotonMap(int totalNumberOfPhotons) : totalNumberOfPhotons(totalNumberOfPhotons) {}
