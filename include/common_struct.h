#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

#include "constants.h"
#include "vectmath.h"

class Map;

struct Vertex {
    int id;
    float x, y;
};

struct Point {
    float x, y, z;
};

struct Line {
    struct Vertex v1;
    struct Vertex v2;
};

struct Wall {
    int id;
    struct Line line;
    float min_height;
    float max_height;
    float floor_height;
    float ceiling_height;
    int color[3];
    bool is_portal;
};

#endif