#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <vector>

#include "common_struct.h"

class Map {
    public:
        
        Map() {}
        
        ~Map() {}

        void AddVertex(int new_id, float x, float y) {
            Vertex vert;
            vert.id = new_id;
            vert.x = x;
            vert.y = y;
            vertices.push_back(vert);
        }

        void AddWall(int id1, int id2, float floorheight, float ceilingheight, int r, int g, int b) {
            Wall wall;
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i].id == id1) wall.line.v1 = vertices[i];
                if (vertices[i].id == id2) wall.line.v2 = vertices[i];
            }
            wall.h_floor = floorheight;
            wall.h_ceiling = ceilingheight;
            wall.color[0] = r;
            wall.color[1] = g;
            wall.color[2] = b;
            walls.push_back(wall);
        }

        std::vector<Wall> walls;
        std::vector<Vertex> vertices;
};

#endif