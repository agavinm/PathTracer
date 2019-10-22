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
    // TODO read image path, method and possibly output path from standard input

    //Image img_hdr = loadPPM("hdr-ppm/seymour_park.ppm");
    Image img_hdr = loadPPM("hdr-ppm/mpi_office.ppm");
    //Image img_hdr = loadPPM("hdr-ppm/mpi_atrium_1.ppm");
    //Image img_hdr = loadPPM("hdr-ppm/mpi_atrium_3.ppm");
    //Image img_hdr = loadPPM("hdr-ppm/nancy_church_1.ppm");
    //Image img_hdr = loadPPM("hdr-ppm/nancy_church_2.ppm");
    //Image img_hdr = loadPPM("hdr-ppm/nancy_church_3.ppm");

    //Image img_ldr = clamping(img_hdr);
    //Image img_ldr = equalization(img_hdr);
    Image img_ldr = equalizeAndClamp(img_hdr, 10000);

    storePPM("pruebas.ppm", img_ldr, 255);

    return 0;
}