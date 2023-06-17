#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

struct Vertex {
    int id;
    float x, y, z;
};

struct Line {
    struct Vertex v1;
    struct Vertex v2;
};

// TODO: maybe implement a wall id?
struct Wall {
    struct Line line;
    float height;
    int color[3];
};

#endif