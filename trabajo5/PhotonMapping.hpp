/******************************************************************************
 * @file    PhotonMapping.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 5: Photon mapping
 ******************************************************************************/

#ifndef TRABAJO5_PHOTONMAPPING_HPP
#define TRABAJO5_PHOTONMAPPING_HPP

#include <mutex>
#include <map>
#include "KDTree.hpp"
#include "Object.hpp"

struct Photon {
    HCoord position; // position of photon
    HCoord direction; // direction from where the photon came
    Color color; // color of the photon
    const Object *object; // Object of the photon
    /* don't put these elements as constants, otherwise the KDTree can't work */
};

/**
 * Container for the photons.
 *
 * Usage:
 * - foreach thread (concurrently):
 * -     generate photons
 * -     call #addAll() to include them in a 'waiting list'
 * - call #markToRead() to convert all the photons in the waiting list to a kdtree
 * - foreach thread (concurrently):
 * -     call #getColorFromMap() as fast as a kdtree allows
 */
class PhotonMap {
    int totalNumberOfPhotons;
    std::map<const Object*, KDTree<Photon, 3>> map;
    std::mutex mtx;

public:

    PhotonMap(int totalNumberOfPhotons);

/**
     * Adds all photons to the 'waiting' list of the container.
     * Can be called multiple times, and is thread-safe
     * When finished adding all photons, don't forget to call #markToRead()
     * @param other list of photon to add
     */
    void addAll(std::vector<Photon> &other);

    /**
     * Returns the color corresponding to the intersection.
     * @param position of the intersection
     * @param direction of where the intersection came
     * @param object object of intersection
     * @return color of that intersection
     */
    Color getColorFromMap(HCoord position, HCoord direction, const Object *object) const;

    /**
     * When called, all photons in the 'waiting' list are converted to a kdtree (removing any existing one) ready to be used with #getColorFromMap()
     * After calling this, following calls to #addAll() or #markToRead() are untested
     */
    void markToRead();

};

#endif //TRABAJO5_PHOTONMAPPING_HPP
