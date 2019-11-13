/******************************************************************************
 * @file    Scenes.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 3
 ******************************************************************************/

#include <cmath>
#include <fstream>
#include <iostream>
#include "Scenes.hpp"

using namespace std;

//////////////////////////////////////////////////////

Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio) {
    return {
            .origin=origin,
            .front=front,
            .left=norm(cross(up, front)) * mod(up) * ratio, // left is ortogonal to up and left, with the size of up*ratio
            .up=up,
    };
}

HCoord getRay(const Camera &camera, float i, float j) {
    return camera.front + camera.left * (1 - 2 * i) + camera.up * (1 - 2 * j);
}

vector<Object> getObjects(const string &scene) {
    vector<Object> objects;
    if (scene == "test") {
        objects.push_back({Sphere(hPoint(10, 0, 0), 1), Texturer(sin2D(C_RED))});
        objects.push_back({Sphere(hPoint(5, 5, 0), 1), Texturer(sin22D(C_GREEN))});
        objects.push_back({Plane(V_AZ, 10), Emitter({0, 0, 1})});
        objects.push_back({Circle(hPoint(5, -5, 0), -V_AX, V_AZ * 2),
                           Texturer(sinCos2D(C_WHITE))});
        objects.push_back({Triangle(hPoint(10, 0, 0), hPoint(5, 5, 0),
                hPoint(5, 0, -5)), Emitter(C_PURPLE)});
    }

    else if (scene == "spiral") {
        int n = 6;
        int m = 10;
        for (int i = 0; i < n * m; ++i) {
            float ang = (float) i / (float) (n * M_PI * 2);
            objects.push_back({Sphere(hPoint((float) -(n * m) / 2 + (float) i, 3 * sin(ang),
                    3 * cos(ang)), 1), Emitter({abs(cos(ang)), abs(sin(ang)),
                                                (float) i / (float) (n * m)})});
        }
    }

    else if (scene == "XYZ") {
        objects.push_back({Sphere(hPoint(3, 0, 0), 1), Emitter(C_RED)});
        objects.push_back({Sphere(hPoint(0, 3, 0), 1), Emitter(C_GREEN)});
        objects.push_back({Sphere(hPoint(0, 0, 3), 1), Emitter(C_BLUE)});
    }

    else { // PLY file
        ifstream file(scene);
        if (!file.is_open()) {
            // can't open, exit
            cerr << "The file " << scene << " can't be opened to read." << endl;
            exit(1);
        }

        bool header = true, vertex = false, face = false;
        string line, field;
        float x, y, z;
        int numVertices = 0, numFaces = 0, vertex1, vertex2, vertex3;
        unsigned char r, g, b;
        vector<pair<HCoord, COLOR>> vertices;
        while (getline(file, line)) {
            if (line.find("comment") != 0 && header) {
                if (line.find("element vertex ") == 0) {
                    numVertices = stoi(line.substr(15));
                    vertices.reserve(numVertices);
                }
                else if (line.find("element face ") == 0) {
                    numFaces = stoi(line.substr(13));
                }
                else if (line == "end_header") {
                    header = false;
                    vertex = true;
                }
            }

            else if (line.find("comment") != 0 && vertex) {
                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                x = stof(field);

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                y = stof(field);

                if (line.find(' ') != string::npos) { // Vertex color
                    field = line.substr(0, line.find(' '));
                    line.erase(0, field.length() + 1);
                    z = stof(field);

                    field = line.substr(0, line.find(' '));
                    line.erase(0, field.length() + 1);
                    r = stoi(field);

                    field = line.substr(0, line.find(' '));
                    line.erase(0, field.length() + 1);
                    g = stoi(field);

                    if (line.find(' ') != string::npos) {
                        field = line.substr(0, line.find(' '));
                        line.erase(0, field.length() + 1);
                    }
                    else {
                        field = line;
                    }
                    b = stoi(field);

                    vertices.emplace_back(hPoint(x, y, z), rgb(r, g, b));
                }
                else { // Not vertex color
                    z = stof(line);
                    vertices.emplace_back(hPoint(x, y, z), C_WHITE);
                }

                if (vertices.size() == numVertices) {
                    vertex = false;
                    face = true;
                }
            }

            else if (line.find("comment") != 0 && face) {
                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                if (stoi(field) != 3) {
                    cerr << scene << " file must be a triangular ply file" << endl;
                    exit(1);
                }

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                vertex1 = stoi(field);

                field = line.substr(0, line.find(' '));
                line.erase(0, field.length() + 1);
                vertex2 = stoi(field);

                vertex3 = stoi(line);

                objects.push_back({Triangle(vertices[vertex1].first, vertices[vertex2].first,
                        vertices[vertex3].first), Emitter((vertices[vertex1].second +
                        vertices[vertex2].second + vertices[vertex3].second) / 3)}); // TODO: En lugar de color de triangulo, pensar en textura dada la posicion dentro del triangulo
            }
        }

        if (!face || objects.size() != numFaces) {
            cerr << scene << " file must be a triangular ply file" << endl;
            exit(1);
        }
    }

    return objects;
}


Camera getCamera(float ratio) {
    return createCamera(
            P_ZERO,
            V_AX,
            V_AZ,
            ratio
    );
}
