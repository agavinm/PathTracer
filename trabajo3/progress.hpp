//
// Created by abeln on 10/11/2019.
//

#ifndef TRABAJO3_PROGRESS_HPP
#define TRABAJO3_PROGRESS_HPP

#include <chrono>

class Progress {
public:
    void start();

    void step(float currentStep);

    void end();

private:
    std::chrono::high_resolution_clock::time_point startTime;
};


#endif //TRABAJO3_PROGRESS_HPP
