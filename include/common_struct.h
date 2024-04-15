#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

#include "constants.h"
#include "vectmath.h"

#include <vector>

class Map;

enum WallType {
    PORTAL,
    WALL
};

enum WallSegment {
    TOP,
    BOTTOM,
    FULL
};

struct Flat {
    int id;
    int color[3];
    float height;
};


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
    WallType type;
};

struct Subsector {
    int id;
    int floor_id, ceiling_id;
    std::vector<Wall*> walls;
};

#endif