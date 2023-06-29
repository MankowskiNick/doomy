#include <math.h>

#include "common_struct.h"
#include "bsp_tree.h"
#include "geom.h"
#include "map.h"

#define ERROR_MARGIN 0.1f

void ExtendDivisionLine(Line& line) {
    float line_dist = Dist(line.v1, line.v2);
    
    if (line_dist < ERROR_MARGIN) 
        return;


    float dx = (line.v2.x - line.v1.x) / line_dist;
    float dy = (line.v2.y - line.v1.y) / line_dist;

    Vertex translation = {
        .id = -1,
        .x = DIVISION_LINE_SCALAR * dx,
        .y = DIVISION_LINE_SCALAR * dy
    };

    Translate(line.v1, translation, -1.0f);
    Translate(line.v2, translation, 1.0f);


}

bool is_front(const Vertex& vertex, const Wall* wall) {
    // Calculate the normal vector of the wall
    float wall_normal_x = wall->line.v2.y - wall->line.v1.y;
    float wall_normal_y = wall->line.v1.x - wall->line.v2.x;
    
    // Calculate the vector from the wall's v1 to the given vertex
    float vertex_vector_x = vertex.x - wall->line.v1.x;
    float vertex_vector_y = vertex.y - wall->line.v1.y;
    
    // Calculate the dot product between the wall normal and the vertex vector
    float dot_product = wall_normal_x * vertex_vector_x + wall_normal_y * vertex_vector_y;
    
    // If the dot product is positive, the vertex is in front of the wall; otherwise, it is not
    return dot_product > 0;
}

bool is_front(Wall& test_wall, Wall* divider_ptr) {
    Vertex midpoint = Midpoint(test_wall);
    return is_front(midpoint, divider_ptr);
}

BSP_Tree* Generate_BSP_Subtree(std::vector<Wall>& walls, Map& map, BSP_Tree* parent) {

    // Nothing to partition, return null
    if (walls.size() == 0)
        return NULL;

    // Create groups of walls in front and walls in back.
    std::vector<Wall> front_walls;
    std::vector<Wall> back_walls;

    // Choose a partition wall
    Wall cur_wall = walls[walls.size() / 2];

    // Create a division line based on the partition wall
    Line division_line = cur_wall.line;
    ExtendDivisionLine(division_line);

    // Partition each wall
    for (int i = 0; i < walls.size(); i++) {

        // Ignore the current wall
        if (walls[i].id == cur_wall.id)
            continue;
            
        // Find the intersection
        Vertex* intersection = FindIntersection(walls[i].line, division_line);

        // If there is no intersection, group the walls accordingly. This is the easy case
        if (intersection == NULL) {
            if (is_front(walls[i], &cur_wall))
                front_walls.push_back(walls[i]);
            else
                back_walls.push_back(walls[i]);
        }

        // Otherwise, we need to split the wall
        else {

            // Grab the id of the current wall
            int id = walls[i].id;

            // Create a back wall
            Wall* back_wall = new Wall;

            // The front wall will replace the wall at map.walls[id]
            map.walls[id].line.v1 = is_front(walls[i].line.v1, &cur_wall) ? walls[i].line.v1 : walls[i].line.v2; // maybe this is the error
            map.walls[id].line.v2 = *intersection;

            back_wall->id = map.walls.size();
            back_wall->line.v1 = is_front(walls[i].line.v1, &cur_wall) ? walls[i].line.v2 : walls[i].line.v1;
            back_wall->line.v2 = *intersection;
            back_wall->height = walls[i].height;
            back_wall->color[0] = walls[i].color[0];
            back_wall->color[1] = walls[i].color[1];
            back_wall->color[2] = walls[i].color[2];

            map.walls.push_back(*back_wall);

            front_walls.push_back(map.walls[id]);
            back_walls.push_back(*back_wall);
        }
    }

    BSP_Tree* return_tree = new BSP_Tree;
    return_tree->root = parent;
    return_tree->id = cur_wall.id;
    return_tree->front = Generate_BSP_Subtree(front_walls, map, return_tree);
    return_tree->back = Generate_BSP_Subtree(back_walls, map, return_tree);
    return return_tree;
}

BSP_Tree Generate_BSP_Tree(std::vector<Wall> walls, Map& map) {
    return *(Generate_BSP_Subtree(walls, map, NULL));
}

std::string BspToString(BSP_Tree* tree, int indent) {
    std::string result;

    // Generate indentation
    for (int i = 0; i < indent; ++i) {
        result += "    ";
    }

    // Append tree ID
    result += std::to_string(tree->id) + "\n";

    // Recursive calls for front and back subtrees
    if (tree->front != nullptr) {
        result += BspToString(tree->front, indent + 1);
    }
    if (tree->back != nullptr) {
        result += BspToString(tree->back, indent + 1);
    }

    return result;
}