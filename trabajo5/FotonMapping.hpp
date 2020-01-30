//
// Created by abeln on 18/01/2020.
//

#ifndef TRABAJO5_FOTONMAPPING_HPP
#define TRABAJO5_FOTONMAPPING_HPP

#include <mutex>
#include <map>
#include "KDTree.hpp"
#include "Object.hpp"

struct Foton {
    HCoord position; // position of foton
    HCoord direction; // direction from where the foton came
    Color color; // color of the foton
    float dist; // distance from the light to the foton
    const Object *object; // Object of the foton
    /* don't put these elements as constants, otherwise the KDTree can't work */
};

/**
 * Container for the fotons.
 *
 * Usage:
 * - foreach thread (concurrently):
 * -     generate fotons
 * -     call #addAll() to include them in a 'waiting list'
 * - call #markToRead() to convert all the fotons in the waiting list to a kdtree
 * - foreach thread (concurrently):
 * -     call #getColorFromMap() as fast as a kdtree allows
 */
class FotonMap {
    std::map<const Object*, KDTree<Foton, 3>> map;
    std::mutex mtx;

public:

    /**
     * Adds all fotons to the 'waiting' list of the container.
     * Can be called multiple times, and is thread-safe
     * When finished adding all fotons, don't forget to call #markToRead()
     * @param other list of foton to add
     */
    void addAll(std::vector<Foton> &other);

    /**
     * Returns the color corresponding to the intersection.
     * @param position of the intersection
     * @param direction of where the intersection came
     * @param object object of intersection
     * @return color of that intersection
     */
    Color getColorFromMap(HCoord position, HCoord direction, float distance, const Object *object) const;

    /**
     * When called, all fotons in the 'waiting' list are converted to a kdtree (removing any existing one) ready to be used with #getColorFromMap()
     * After calling this, following calls to #addAll() or #markToRead() are untested
     */
    void markToRead();

};

#endif //TRABAJO5_FOTONMAPPING_HPP
