/******************************************************************************
 * @file    Scene.cpp
 * @author  Andrés Gavín Murillo, 716358
 * @author  Abel Naya Forcano, 544125
 * @date    Noviembre 2019
 * @coms    Informática Gráfica - Trabajo recomendado 4
 ******************************************************************************/

#include <cmath>
#include <fstream>
#include <iostream>
#include "Scene.hpp"
#include "Texture.hpp"

using namespace std;

//////////////////////////////////////////////////////

Camera createCamera(const HCoord &origin, const HCoord &front, const HCoord &up, float ratio) {
    return {
            .origin=origin,
            .front=front,
            .left=norm(cross(up, front)) * mod(up) * ratio, // left is ortogonal to up and front, with the size of up*ratio
            .up=up
    };
}

HCoord getRay(const Camera &camera, float i, float j) {
    return camera.front + camera.left * (1 - 2 * i) + camera.up * (1 - 2 * j);
}

Scene defaultScene(float ratio) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<LightPoint> lightPoints;
    lightPoints.push_back({
                                  C_WHITE,
                                  hPoint(5, 5, 5)
                          });

    vector<Object> objects;

    // BOX:
    objects.push_back({
                              Plane(hVector(-1, 0, 0), 5),
                              Reflector(colored(C_GREY), colored(C_BLACK), 0.0f)
                      }); // FRONT
    objects.push_back({
                              Plane(hVector(0, 1, 0), 5),
                              Reflector(colored(C_GREEN), colored(C_BLACK), 0.0f)
                      }); // RIGHT
    objects.push_back({
                              Plane(hVector(0, -1, 0), 5),
                              Reflector(colored(C_RED), colored(C_BLACK), 0.0f)
                      }); // LEFT
    objects.push_back({
                              Plane(hVector(0, 0, 1), 5),
                              Reflector(colored(C_GREY), colored(C_BLACK), 0.0f)
                      }); // DOWN
    objects.push_back({
                              Plane(hVector(0, 0, -1), 5),
                              Reflector(colored(C_GREY), colored(C_BLACK), 0.0f)
                      }); // UP

    // SPHERES:
    objects.push_back({
                              Sphere(hPoint(3, 3, -4), 1),
                              Reflector(colored(C_BLUE), colored(C_GREEN), 1.0f)
                      });
    objects.push_back({
                              Sphere(hPoint(3, -3, -3), 2),
                              Reflector(colored(C_BLUE), colored(C_GREEN), 0.0f)
                      });

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints
    };
}

Scene testScene(float ratio) {
    Camera camera = createCamera(P_ZERO, V_AX, V_AZ, ratio);

    vector<LightPoint> lightPoints;
    lightPoints.push_back({
        C_WHITE,
        hPoint(5, 0, -5)
    });

    vector<Object> objects;
    objects.push_back({
                              Sphere(hPoint(10, 0, 0), 1),
                              Emitter(sin2D(C_RED))
                      });
    objects.push_back({
                              Sphere(hPoint(5, 5, 0), 1),
                              Emitter(sin22D(C_GREEN))
                      });
    objects.push_back({
                              Plane(V_AZ, 10),
                              Emitter(colored({0, 0, 1}))
                      });
    objects.push_back({
                              Circle(hPoint(5, -5, 0), -V_AX, V_AZ * 2),
                              Emitter(sinCos2D(C_WHITE))
                      });

    HCoord points[3] = {hPoint(10, 0, 0), hPoint(5, 5, 0), hPoint(5, 0, -5)};
    Color colors[3] = {C_RED, C_BLUE, C_GREEN};
    objects.push_back({
                              Triangle(points[0], points[1], points[2]),
                              Emitter(sinCos2D(vertexColorDistanceWeightingSquare(colors, points)))
                      });
    objects.push_back({
                              Cuadric(-1, -1, 1, 0, 0, 0, 0, 0, 0, -1),
                              Emitter(colored(C_CYAN))
                      });

    return {
        .camera = camera,
        .objects = objects,
        .lightPoints = lightPoints
    };
}

Scene plyScene(const string &scene, float ratio) { // TODO: Light points??
    Camera camera = createCamera(P_ZERO, V_AX, V_AZ, ratio);

    vector<Object> objects;

    ifstream file(scene);
    if (!file.is_open()) {
        // can't open, exit
        cerr << "The file " << scene << " can't be opened to read." << endl;
        exit(1);
    }

    bool header = true, vertex = false, face = false, color = false;
    string line, field;
    float x, y, z;
    int numVertices = 0, numFaces = 0, vertex1, vertex2, vertex3;
    unsigned char r, g, b;
    vector<pair<HCoord, Color>> vertices;
    while (getline(file, line)) {
        if (line.size() && line[line.size() - 1] == '\r') {
            line = line.substr(0, line.size() - 1);
        }
        if (line.find("comment") != 0 && header) {
            if (line.find("element vertex ") == 0) {
                numVertices = stoi(line.substr(15));
                vertices.reserve(numVertices);
            } else if (line.find("element face ") == 0) {
                numFaces = stoi(line.substr(13));
            } else if (line == "end_header") {
                header = false;
                vertex = true;
            }
        } else if (line.find("comment") != 0 && vertex) {
            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            x = stof(field);

            field = line.substr(0, line.find(' '));
            line.erase(0, field.length() + 1);
            y = stof(field);

            if (line.find(' ') != string::npos) { // Vertex color
                color = true;

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
                } else {
                    field = line;
                }
                b = stoi(field);

                vertices.emplace_back(hPoint(x, y, z), rgb(r, g, b));
            } else { // Not vertex color
                z = stof(line);
                vertices.emplace_back(hPoint(x, y, z), C_WHITE);
            }

            if (vertices.size() == numVertices) {
                vertex = false;
                face = true;
            }
        } else if (line.find("comment") != 0 && face) {
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

            Color col[3] = {vertices[vertex1].second,
                            vertices[vertex2].second, vertices[vertex3].second};

            HCoord vert[3] = {vertices[vertex1].first, vertices[vertex2].first,
                              vertices[vertex3].first};

            if (color) {
                objects.push_back({Triangle(vertices[vertex1].first, vertices[vertex2].first,
                                            vertices[vertex3].first),
                                   Emitter(colored(vertexColorDistanceWeightingSquare(col, vert)))});
            } else {
                objects.push_back({Triangle(vertices[vertex1].first, vertices[vertex2].first,
                                            vertices[vertex3].first), Emitter(colored(C_WHITE))});
            }
        }
    }

    if (!face || objects.size() != numFaces) {
        cerr << scene << " file must be a triangular ply file" << endl;
        exit(2);
    }

    return {
            .camera = camera,
            .objects = objects
    };
}

Scene createScene(const string &scene, float ratio) {
    if (scene == "default")
        return defaultScene(ratio);
    else if (scene == "test")
        return testScene(ratio);
    else
        return plyScene(scene, ratio);
}
