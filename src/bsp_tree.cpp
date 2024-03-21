#include <math.h>
#include<string>

#include "common_struct.h"
#include "bsp_tree.h"
#include "geom.h"
#include "map.h"

DivLine GetDivLineFromWorldLine(Line& line) {
    return {
        .vert = line.v1,
        .dx = line.v2.x - line.v1.x,
        .dy = line.v2.y - line.v1.y
    };
}

WallSide VertOnSide(DivLine& div_line, Vertex& v) {

    // Calculate normal vector of div_line
    float n_dx, n_dy;
    n_dx = -1 * div_line.dy;
    n_dy = div_line.dx;

    // Get vector from div_line.vert to v
    float dv_dx, dv_dy;
    dv_dx = div_line.vert.x - v.x;
    dv_dy = div_line.vert.y - v.y;

    // Calculate dot product
    float dp;
    dp = dv_dx * n_dx + dv_dy * n_dy;

    if (dp == 0)
        return INTERSECT;
    else if (dp > 0)
        return FRONT;
    else
        return BACK;
}

BSP_Tree* _DeserializeBSPHelper(const std::string& bsp_string, int& pos) {

    // Base case, we have reached a leaf
    if (bsp_string[pos] == '#') {
        pos++;
        return NULL;
    }

    // Read id
    pos += 4; // Skip past (id=
    std::string id_str = "";
    while (bsp_string[pos] != ',')
        id_str += bsp_string[pos++];
    int id = std::stoi(id_str);

    // Read the wall id
    pos += 9; // Skip past ,wall_id=
    std::string wall_id_str = "";
    while (bsp_string[pos] != ',')
        wall_id_str += bsp_string[pos++];
    int wall_id = std::stoi(wall_id_str);


    // Read the front node
    pos += 7; // Skip past ,front=
    BSP_Tree* front_subtree = _DeserializeBSPHelper(bsp_string, pos);

    // Read the back node
    pos += 6; // Skip past ,back=
    BSP_Tree* back_subtree = _DeserializeBSPHelper(bsp_string, pos);

    // Skip past the closing )
    pos++;

    // Assemble the root node
    BSP_Tree* root_tree = new BSP_Tree;
    root_tree->id = id;
root_tree->wall_id = wall_id;
    root_tree->front = front_subtree;
    root_tree->back = back_subtree;

    // Return it
    return root_tree;
}

BSP_Tree DeseralizeBSP(const std::string& bsp_string) {
    int pos = 0;
    return *(_DeserializeBSPHelper(bsp_string, pos));
}