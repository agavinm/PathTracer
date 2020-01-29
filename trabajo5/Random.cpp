//
// Created by abeln on 29/01/2020.
//

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
