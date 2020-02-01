/******************************************************************************
 * @file    Random.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#include "Random.hpp"
#include <random>

using namespace std;

static thread_local random_device rd;
static thread_local std::mt19937 generator(rd());
static thread_local uniform_real_distribution<float> dist(0.0f, nextafter(1.0f, MAXFLOAT));


float random_zero_one() {
    return dist(generator);
}

int random_zero_n(int n) {
    return uniform_int_distribution<>(0, n)(generator);
}
