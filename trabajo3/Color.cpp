/******************************************************************************
 * @file    Color.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <algorithm>
#include "Color.hpp"

using namespace std;

float Color::max() const {
    return std::max(std::max(this->r, this->g), this->b);
}

Color Color::operator+(const Color &right) const {
    return {
            this->r + right.r,
            this->g + right.g,
            this->b + right.b
    };
}

Color Color::operator/(float s) const {
    return {
            this->r / s,
            this->g / s,
            this->b / s
    };
}

Color Color::operator*(float s) const {
    return {
            this->r * s,
            this->g * s,
            this->b * s
    };
}

Color rgb(const unsigned char r, const unsigned char g, const unsigned char b) {
    return {(float) r / 255.0f, (float) g / 255.0f, (float) b / 255.0f};
}

VertexColor vertexColor(const Color colors[3], const HCoord vertices[3], const VertexColor::VERTEX_COLOR_TYPE &type) {
    return VertexColor {
            .type = type,
            .colors = {colors[0], colors[1], colors[2]},
            .vertices = {vertices[0], vertices[1], vertices[2]}
    };
}

VertexColor vertexColorNearest(const Color colors[3], const HCoord vertices[3]) {
    return vertexColor(colors, vertices, VertexColor::NEAREST);
}

VertexColor vertexColorDistanceWeighting(const Color colors[3], const HCoord vertices[3]) {
    return vertexColor(colors, vertices, VertexColor::DISTANCE_WEIGHTING);
}

VertexColor vertexColorDistanceWeightingSquare(const Color colors[3], const HCoord vertices[3]) {
    return vertexColor(colors, vertices, VertexColor::DISTANCE_WEIGHTING_SQUARE);
}

Color getColor(const VertexColor &vertexColor, const HCoord &position) {
    float mods[3] = {mod(position - vertexColor.vertices[0]),
                     mod(position - vertexColor.vertices[1]),
                     mod(position - vertexColor.vertices[2])};

    if (mods[0] == 0.0f) return vertexColor.colors[0];
    else if (mods[1] == 0.0f) return vertexColor.colors[1];
    else if (mods[2] == 0.0f) return vertexColor.colors[2];

    switch (vertexColor.type) {
        case VertexColor::NEAREST: {
            if (mods[0] < mods[1] && mods[0] < mods[2]) return vertexColor.colors[0];
            else if (mods[1] < mods[0] && mods[1] < mods[2]) return vertexColor.colors[1];
            else return vertexColor.colors[2];
        }
        case VertexColor::DISTANCE_WEIGHTING: {
            return ((vertexColor.colors[0] / mods[0] + vertexColor.colors[1] / mods[1] + vertexColor.colors[2] / mods[2])
                    / (1.0f / mods[0] + 1.0f / mods[1] + 1.0f / mods[2]));
        }
        case VertexColor::DISTANCE_WEIGHTING_SQUARE: {
            return ((vertexColor.colors[0] / (mods[0] * mods[0]) + vertexColor.colors[1] / (mods[1] * mods[1])
                     + vertexColor.colors[2] / (mods[2] * mods[2]))
                    / (1.0f / (mods[0] * mods[0]) + 1.0f / (mods[1] * mods[1]) + 1.0f / (mods[2] * mods[2])));
        }
        default:
            break;
    }
}