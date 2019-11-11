/******************************************************************************
 * @file    Progress.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#ifndef TRABAJO3_PROGRESS_HPP
#define TRABAJO3_PROGRESS_HPP

#include <string>
#include <chrono>

class Progress {
public:
    void start(const std::string &prefix);

    void step(float currentStep);

    void end();

private:
    std::chrono::high_resolution_clock::time_point startTime;
    std::string prefix;
};

#endif //TRABAJO3_PROGRESS_HPP
