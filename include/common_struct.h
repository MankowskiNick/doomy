#ifndef COMMON_STRUCT_H
#define COMMON_STRUCT_H

struct Vertex {
    int id;
    float x, y;
};

struct Line {
    Vertex v1;
    Vertex v2;
};

// TODO: maybe implement a wall id?
struct Wall {
    Line line;
    float h_floor, h_ceiling;
    int color[3];
};


#endif