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

using namespace std;

int main() {

    // input data somehow
    HCoord centerPlanetA;
    HCoord axisPLanetA;
    HCoord referenceCityPlanetA;
    float inclinationStationA;
    float azimutStationA;
    HCoord centerPlanetB;
    HCoord axisPLanetB;
    HCoord referenceCityPlanetB;
    float inclinationStationB;
    float azimutStationB;

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
    if (dirAtoB.z() < 0) {
        cout << "._. I'm afraid I can't do that Dave";
    }

    cout << "^.^ From the station B you need to point the receiver to local coordinates: " << dirBtoA << endl;
    if (dirBtoA.z() < 0) {
        cout << " O_o I Can't Let You Do That Starfox";
    }

    return 0;
}