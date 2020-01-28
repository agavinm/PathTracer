//
// Created by abeln on 18/01/2020.
//

#ifndef TRABAJO5_FOTONMAPPING_HPP
#define TRABAJO5_FOTONMAPPING_HPP

#include <mutex>
#include "KDTree.hpp"

struct Foton {
    HCoord position; // position of foton
    HCoord direction; // direction from where the foton came
    Color color; // color of the foton
    float dist; // distance to the light
    /* don't put these elements as constants, otherwise the KDTree can't work */
};

/**
 * Currently implemented as a list of all fotons. Change to hashing, or something similar
 */
class FotonMap {

    KDTree<Foton, 3> tree;
    std::mutex mtx;

public:

    void addAll(std::vector<Foton> &other);

    Color getColorFromMap(HCoord position, HCoord direction, float dist) const;

    void markToRead();
};

#endif //TRABAJO5_FOTONMAPPING_HPP
