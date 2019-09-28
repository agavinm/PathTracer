//******************************************************************************
// File:   Geometric.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Septiembre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 1
//******************************************************************************

#include <Geometric.hpp>
#include <cmath>

Planet createPlanet(const HCoord &center, const HCoord &axis, const HCoord &city) {
    Planet planet;

    // set the center of the sphere
    planet.sphere.center = center;

    // set the radius of the sphere
    HCoord cityVect = city - center;
    planet.sphere.radius = mod(city - center);
    if (fabsf(mod(axis) / 2 - planet.sphere.radius) > 1e-6) {
        throw "Radius of planet can't be deduced: axis module is not double of city distance to center";
    }

    // create axisX
    planet.axisX = cross(axis, cityVect);
    if (planet.axisX == point(0, 0, 0)) {
        throw "City can't be on the poles";
    }
    planet.axisX = norm(planet.axisX);

    // create axisY
    planet.axisY = norm(cross(axis, planet.axisX));

    // return
    return planet;
}

Station getStation(const Planet &planet, float inclination, float azimut) {
    Station station;

    // create transformation
    Transform transform = changeFromBase(planet.axisX, planet.axisY, cross(planet.axisX, planet.axisY), planet.sphere.center)
                          * rotationZ(azimut)
                          * rotationX(inclination);

    // apply transformation
    station.position = transform * point(0, 0, planet.sphere.radius);
    station.axisX = transform * vector(1, 0, 0);
    station.axisY = transform * vector(0, 1, 0);

    return station;
}


HCoord getLocalDirection(const Station &local, const Station &other) {
    // translate position of other to local basei
    return norm(vectorify(
            changeToBase(local.axisX, local.axisY, cross(local.axisX, local.axisY), local.position) * other.position
    ));
}
