#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

#include "constants.h"
#include "vectmath.h"

class Map;

struct Vertex {
    int id;
    float x, y, z;
};

struct Line {
    struct Vertex v1;
    struct Vertex v2;
};

struct Wall {
    int id;
    struct Line line;
    float height;
    int color[3];
};

Vect2<float> VertToVect2(const Vertex& vert);

bool VertexEquals(const Vertex& v1, const Vertex& v2);

#endif