//******************************************************************************
// File:   main.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//******************************************************************************

#include "HCoord.hpp"
#include "Geometric.hpp"
#include <iostream>
#include <math.h>

using namespace std;

int main() {

    // input data somehow
    HCoord centerPlanetA = point(0, 0, 0);
    HCoord axisPLanetA = vector(0, 0, 2);
    HCoord referenceCityPlanetA = point(0, 1, 0);
    float inclinationStationA = M_PI_2;
    float azimutStationA = 0;

    HCoord centerPlanetB = point(100, 0, 0);
    HCoord axisPLanetB = vector(0, 0, 2);
    HCoord referenceCityPlanetB = point(100, 1, 0);
    float inclinationStationB = M_PI_2;
    float azimutStationB = M_PI;

    // create Station
    Station stationA = getStation(createPlanet(centerPlanetA, axisPLanetA, referenceCityPlanetA), inclinationStationA,
                                  azimutStationA);
    Station stationB = getStation(createPlanet(centerPlanetB, axisPLanetB, referenceCityPlanetB), inclinationStationB,
                                  azimutStationB);

    // getVectors
    HCoord dirAtoB = getLocalDirection(stationA, stationB);
    HCoord dirBtoA = getLocalDirection(stationB, stationA);

    // print
    cout
            << "·¬· From the station A you need to point the death, I mean, the quantum catapult ray to local coordinates: "
            << dirAtoB << endl;
    if (dirAtoB.e[2] < -1e-6) {
        cout << "._. I'm afraid I can't do that Dave" << endl;
    }

    cout << endl;

    cout << "^.^ From the station B you need to point the receiver to local coordinates: " << dirBtoA << endl;
    if (dirBtoA.e[2] < -1e-6) {
        cout << " O_o I Can't Let You Do That Starfox" << endl;
    }

    return 0;
}