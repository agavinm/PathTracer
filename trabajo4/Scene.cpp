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

    vector<Object> objects;

    // LIGHT:
    vector<LightPoint> lightPoints;
    lightPoints.push_back(createLightPoint(C_WHITE,hPoint(2.5f,0,-4)));

    objects.push_back(create2D(
            Plane(hVector(0, 0, -1), 5),
            Emitter(colored(C_WHITE))
    )); // UP

    // BOX:
    objects.push_back(create2D(
                              Plane(hVector(-1, 0, 0), 5),
                              Diffuse(colored(C_GREY))
                      )); // FRONT
    objects.push_back(create2D(
                              Plane(hVector(0, 1, 0), 5),
                              Diffuse(colored(C_GREEN))
                      )); // RIGHT
    objects.push_back(create2D(
                              Plane(hVector(0, -1, 0), 5),
                              Diffuse(colored(C_RED))
                      )); // LEFT
    objects.push_back(create2D(
                              Plane(hVector(0, 0, 1), 5),
                              Diffuse(colored(C_GREY))
                      )); // DOWN

    // SPHERES:
    objects.push_back(create3D(
                              Sphere(hPoint(3, 2.5f, -2.5f), 1.5f),
                              Phong(colored(C_BLUE), colored(C_GREEN), 10),
                              VACUUM_REFRACTIVE_INDEX
                      ));
    objects.push_back(create3D(
                              Sphere(hPoint(3, -3, -3), 2),
                              Delta(colored(C_BLUE), colored(C_YELLOW)), // Refracts blue and reflects yellow
                              WATER_REFRACTIVE_INDEX
                      ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = lightPoints,
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

Scene specularScene(float ratio) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    objects.push_back(create2D(
            Plane(hVector(0, 0, -1), 5),
            Emitter(colored(C_WHITE))
    )); // UP

    // MIRROR
    objects.push_back(create2D(
            Triangle(hPoint(5, 0, 2), hPoint(5, 2, -2), hPoint(5, -2, -2)),
            Specular(colored(C_WHITE))
    )); // FRONT

    // BOX:
    objects.push_back(create2D(
            Plane(hVector(-1, 0, 0), 5),
            Diffuse(colored(C_GREY))
    )); // FRONT
    objects.push_back(create2D(
            Plane(hVector(0, 1, 0), 5),
            Diffuse(colored(C_GREEN))
    )); // RIGHT
    objects.push_back(create2D(
            Plane(hVector(0, -1, 0), 5),
            Diffuse(colored(C_RED))
    )); // LEFT
    objects.push_back(create2D(
            Plane(hVector(0, 0, 1), 5),
            Diffuse(colored(C_GREY))
    )); // DOWN

    // SPHERES:
    objects.push_back(create3D(
            Sphere(hPoint(3, 4, -4), 1),
            Phong(colored(C_BLUE), colored(C_GREEN), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(3, -3, -3), 2),
            Specular(colored(C_WHITE)),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(2, 3, 3), 1.5f),
            Specular(colored(C_WHITE)),
            VACUUM_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

Scene refractionScene(float ratio) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    objects.push_back(create2D(
            Plane(hVector(0, 0, -1), 5),
            Emitter(colored(C_WHITE))
    )); // UP

    // BOX:
    objects.push_back(create2D(
            Plane(hVector(-1, 0, 0), 5),
            Diffuse(colored(C_GREY))
    )); // FRONT
    objects.push_back(create2D(
            Plane(hVector(0, 1, 0), 5),
            Diffuse(colored(C_GREEN))
    )); // RIGHT
    objects.push_back(create2D(
            Plane(hVector(0, -1, 0), 5),
            Diffuse(colored(C_RED))
    )); // LEFT
    objects.push_back(create2D(
            Plane(hVector(0, 0, 1), 5),
            Diffuse(colored(C_GREY))
    )); // DOWN

    // SPHERES:
    objects.push_back(create3D(
            Sphere(hPoint(3, -3, -3), 1),
            Phong(colored(C_GREEN), colored(C_YELLOW), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(3, -3, -3), 2),
            Refractor(colored(C_CYAN)),
            LIQUID_HELIUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(3, 2.5f, -2), 1.5f),
            Phong(colored(C_BLUE), colored(C_GREEN), 2),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(3, 3, -3), 2),
            Refractor(colored(C_YELLOW)),
            AMBER_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 4.0f
    };
}

Scene circleScene(float ratio) {
    Camera camera = createCamera(hPoint(-5, 0, 0), V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    objects.push_back(create2D(
            Circle(hPoint(2, 0, 5), V_AX*2, V_AY*2),
            Emitter(colored(C_WHITE))
    )); // UP

    // BOX:
    objects.push_back(create2D(
            Plane(hVector(-1, 0, 0), 5),
            Diffuse(colored(C_GREY))
    )); // FRONT
    objects.push_back(create2D(
            Plane(hVector(0, 1, 0), 5),
            Diffuse(colored(C_GREEN))
    )); // RIGHT
    objects.push_back(create2D(
            Plane(hVector(0, -1, 0), 5),
            Diffuse(colored(C_RED))
    )); // LEFT
    objects.push_back(create2D(
            Plane(hVector(0, 0, 1), 5),
            Diffuse(colored(C_GREY))
    )); // DOWN
    objects.push_back(create2D(
            Plane(hVector(0, 0, -1), 5),
            Diffuse(colored(C_GREY))
    )); // UP

    // SPHERES:
    objects.push_back(create3D(
            Sphere(hPoint(3, 2.5f, -2.5f), 1.5f),
            Phong(colored(C_BLUE), colored(C_GREEN), 10),
            VACUUM_REFRACTIVE_INDEX
    ));
    objects.push_back(create3D(
            Sphere(hPoint(3, -3, -3), 2),
            Delta(colored(C_BLUE), colored(C_YELLOW)), // Refracts blue and reflects yellow
            WATER_REFRACTIVE_INDEX
    ));

    return {
            .camera = camera,
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 5.0f
    };
}

Scene testScene(float ratio) {
    Camera camera = createCamera(P_ZERO, V_AX, V_AZ, ratio);

    vector<Object> objects;

    // LIGHT:
    objects.push_back({
                              Triangle(hPoint(2.5f, 2.5f, 5), hPoint(2.5f, -2.5f, 5), hPoint(-2.5f, 2.5f, 5)),
                              Emitter(colored(C_WHITE))
                      });

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
            .lightPoints = {},
        .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
        .gammaCorrection = 1.0f
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
            .objects = objects,
            .lightPoints = {},
            .refractiveIndex = VACUUM_REFRACTIVE_INDEX,
            .gammaCorrection = 1.0f
    };
}

Scene createScene(const string &scene, float ratio) {
    if (scene == "default")
        return defaultScene(ratio);
    else if (scene == "test")
        return testScene(ratio);
    else if (scene == "specular")
        return specularScene(ratio);
    else if (scene == "refraction")
        return refractionScene(ratio);
    else if (scene == "circle")
        return circleScene(ratio);
    else
        return plyScene(scene, ratio);
}
