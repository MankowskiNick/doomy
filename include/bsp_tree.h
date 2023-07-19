#ifndef BSP_TREE_H
#define BSP_TREE_H

#include <vector>
#include "common_struct.h"
//#include "map.h"

#include <iostream>

class Map;

#define DIVISION_LINE_SCALAR 1000.0f

struct BSP_Tree {
    int id;
    BSP_Tree* root;
    BSP_Tree* front;
    BSP_Tree* back;
};

BSP_Tree Generate_BSP_Tree(std::vector<Wall> walls, Map& map);
// BSP_Tree* Generate_BSP_Tree(const std::vector<Wall>& walls);

bool is_front(const Vertex& test, const Wall* divider_ptr);

#endif