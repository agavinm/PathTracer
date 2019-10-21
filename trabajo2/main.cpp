//******************************************************************************
// File:   main.cpp
// Author: Andrés Gavín Murillo 716358
// Author: Abel Naya Forcano 544125
// Date:   Octubre 2019
// Coms:   Informática Gráfica - Trabajo recomendado 2
//******************************************************************************

#include <iostream>
#include "Image.hpp"

using namespace std;

int main() {
    Image image = loadPPM("hdr-ppm/mpi_atrium_1.ppm");
    storePPM("pruebas.ppm", image);
    return 0;
}