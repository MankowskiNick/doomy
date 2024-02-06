#include <math.h>
#include<string>

#include "common_struct.h"
#include "bsp_tree.h"
#include "geom.h"
#include "map.h"

bool is_front(const Vertex& vertex, const Wall* wall) {
    // Look at z component of cross product of wall and vertex
    Vect2<float> line_vect = {
        .a = wall->line.v2.x - wall->line.v1.x,
        .b = wall->line.v2.y - wall->line.v1.y,
    };    
    
    Vect2<float> v1_to_vert_vect = {
        .a = vertex.x - wall->line.v1.x,
        .b = vertex.y - wall->line.v1.y,
    };

    float cross_product_z = CrossProductZ(line_vect, v1_to_vert_vect);

    return cross_product_z > 0;
}

bool is_front(Wall& test_wall, Wall* divider_ptr) {
    Vertex midpoint = Midpoint(test_wall);
    return is_front(midpoint, divider_ptr);
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
    root_tree->front = front_subtree;
    root_tree->back = back_subtree;

    // Return it
    return root_tree;
}

BSP_Tree DeseralizeBSP(const std::string& bsp_string) {
    int pos = 0;
    return *(_DeserializeBSPHelper(bsp_string, pos));
}