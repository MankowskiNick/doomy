#ifndef WALL_NODE_H
#define WALL_NODE_H

#include "bsp_tree.h"
#include "map.h"

struct Wall_Node {
    int id;
    Wall_Node* next;
    Wall_Node* previous;
};

Wall_Node* GetLast(Wall_Node* head);

Wall_Node* GetFirst(Wall_Node* head);

Wall_Node* FindOrder(BSP_Tree* bsp_tree, Map& map);

#endif