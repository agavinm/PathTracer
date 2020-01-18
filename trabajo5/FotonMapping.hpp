//
// Created by abeln on 18/01/2020.
//

#ifndef TRABAJO5_FOTONMAPPING_HPP
#define TRABAJO5_FOTONMAPPING_HPP

#include <mutex>

struct Foton {
    const HCoord position; // position of foton
    const HCoord direction; // direction from where the foton came
    const Color color; // color of the foton
    const float dist; // distance to the light
};

/**
 * Currently implemented as a list of all fotons. Change to hashing, or something similar
 */
class FotonMap {

    std::vector<Foton> map;
    std::mutex mtx;

public:

    void addAll(FotonMap &other);

    Color getColorFromMap(HCoord position, HCoord direction, float dist) const;

    void addFoton(Foton foton);
};

#endif //TRABAJO5_FOTONMAPPING_HPP
