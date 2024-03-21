#ifndef BSP_TREE_H
#define BSP_TREE_H

#include <vector>
#include "common_struct.h"

#include <iostream>

class Map;

#define	SUBSECTOR 0x8000

struct DivLine {
    Vertex vert;
    float dx, dy;
};

enum WallSide {
    COINCIDENT = -1,
    INTERSECT  = -2,
    FRONT      =  0,
    BACK       =  1,
};

struct BSP_Tree {
    int id;
    int wall_id;
    BSP_Tree* root;
    BSP_Tree* front;
    BSP_Tree* back;
};

WallSide VertOnSide(DivLine& div_line, Vertex& v);

DivLine GetDivLineFromWorldLine(Line& line);

BSP_Tree DeseralizeBSP(const std::string& bsp_string);

#endif