/******************************************************************************
 * @file    Progress.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Enero 2020
 * @coms    Informática Gráfica - Trabajo 4: Path tracer
 ******************************************************************************/

#include "Progress.hpp"
#include <iostream>
#include <iomanip>

void Progress::start(const std::string &prefix) {
    using namespace std::chrono;
    this->startTime = high_resolution_clock::now();
    this->prefix = prefix;
    this->step(0);
}

void Progress::step(float currentStep) {
    using namespace std;
    cout << "\r" << this->prefix << " progress : " << fixed << setprecision(3) << currentStep << "%" << flush;
}

void Progress::end() {
    using namespace std::chrono;
    auto stop = high_resolution_clock::now();
    auto dur = stop - this->startTime;
    auto h = duration_cast<hours>(dur);
    auto m = duration_cast<minutes>(dur -= h);
    auto s = duration_cast<seconds>(dur -= m);
    auto ms = duration_cast<milliseconds>(dur -= s);

    using namespace std;

    cout << "\r" << this->prefix << " finished in ";
    if (h.count() != 0) {
        cout << h.count() << "d ";
    }
    if (m.count() != 0) {
        cout << m.count() << "m ";
    }
    cout << s.count() << "," << setfill('0') << setw(3) << ms.count() << "s";
    cout << endl;

}
