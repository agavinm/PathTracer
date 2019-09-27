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

    // create longitudeDir
    planet.longitudeDir = cross(axis, cityVect); // WARNING: maybe needs to be negated
    if (planet.longitudeDir == zeroHCoord()) { //HCoord.ZERO) {
        throw "City can't be on the poles";
    }
    planet.longitudeDir = norm(planet.longitudeDir);

    // create latitudeDir
    planet.latitudeDir = norm(cross(axis, planet.longitudeDir)); // WARNING: maybe needs to be negated

    // return
    return planet;
}

Station getStation(const Planet &planet, float inclination, float azimut) {
    Station station;

    // create transformation
    Transform transform = translation(planet.sphere.center) * rotationX(azimut) * rotationY(inclination); // WARNING: maybe azimut and/or inclination needs to be negated

    // apply transformation
    station.position = transform * point(0, 0, planet.sphere.radius); //HCoord(0, 0, planet.sphere.radius);
    station.latitudeDir = transform * planet.latitudeDir;
    station.longitudeDir = transform * planet.longitudeDir;

    return station;
}


HCoord getLocalDirection(const Station &local, const Station &other) {
    return changeBase(local.longitudeDir, local.latitudeDir, cross(local.longitudeDir, local.latitudeDir), local.position) * other.position;
}
