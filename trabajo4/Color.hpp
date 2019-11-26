/******************************************************************************
 * @file    Color.hpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#ifndef TRABAJO3_COLOR_HPP
#define TRABAJO3_COLOR_HPP

#include "HCoord.hpp"

struct Color {
    float r;
    float g;
    float b;

    float max() const;

    Color operator+(const Color &right) const;

    Color operator/(float s) const;

    Color operator*(float s) const;

};

struct VertexColor {
    enum VERTEX_COLOR_TYPE {
        NEAREST,
        DISTANCE_WEIGHTING,
        DISTANCE_WEIGHTING_SQUARE
    };
    VERTEX_COLOR_TYPE type;

    const Color colors[3];
    const HCoord vertices[3];
};

Color rgb(unsigned char r, unsigned char g, unsigned char b);

VertexColor vertexColorNearest(const Color colors[3], const HCoord vertices[3]);

VertexColor vertexColorDistanceWeighting(const Color colors[3], const HCoord vertices[3]);

VertexColor vertexColorDistanceWeightingSquare(const Color colors[3], const HCoord vertices[3]);

Color getColor(const VertexColor &vertexColor, const HCoord &position);

// color constants
static const Color C_BLACK = {0, 0, 0};
static const Color C_RED = {1, 0, 0};
static const Color C_GREEN = {0, 1, 0};
static const Color C_BLUE = {0, 0, 1};
static const Color C_CYAN = {0, 1, 1};
static const Color C_PURPLE = {1, 0, 1};
static const Color C_YELLOW = {1, 1, 0};
static const Color C_WHITE = {1, 1, 1};
static const Color C_GREY = {0.5f, 0.5f, 0.5f};

#endif //TRABAJO3_COLOR_HPP
