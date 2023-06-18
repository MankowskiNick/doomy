#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <vector>

#include "common_struct.h"

class Map {
    public:
        
        Map() {}
        
        ~Map() {}

        void AddVertex(int id, float x, float y, float z) {
            Vertex vert = {
                .id = id,
                .x = x,
                .y = y,
                .z = z
            };

            vertices.push_back(vert);
        }

        void AddWall(int id1, int id2, float wall_height, int r, int g, int b) {
            Wall wall;
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i].id == id1) wall.line.v1 = vertices[i];
                if (vertices[i].id == id2) wall.line.v2 = vertices[i];
            }
            wall.height = wall_height;
            wall.color[0] = r;
            wall.color[1] = g;
            wall.color[2] = b;
            walls.push_back(wall);
        }

        void LoadFile(char* file_name) {

            // File format: 
            //     [verts]
            //     v1.id v1.x v1.y v1.z
            //     ...
            //     [walls]
            //     v1.id v2.id height r g b
            //     ...
            try {
                std::ifstream file_stream(file_name);

                std::string cur_line_header = "";

                int skiplines = 0;
                while (cur_line_header != "[verts]") {
                    skiplines++;
                    if (skiplines > 1000) 
                        throw;
                    file_stream >> cur_line_header;
                }
                file_stream >> cur_line_header;

                while (cur_line_header != "[walls]") {

                    // Declare variables needed
                    int id;
                    float x, y, z;

                    // Input data values
                    file_stream >> id;
                    file_stream >> x;
                    file_stream >> y;
                    file_stream >> z;

                    // Add the vertex to the map
                    AddVertex(id, x, y, z);

                    file_stream >> cur_line_header;
                }

                file_stream >> cur_line_header;

                while (cur_line_header != "[end]") {

                    // Declare variables needed
                    int id1, id2; 
                    float height;
                    int r, g, b;

                    // Input the data values
                    file_stream >> id1;
                    file_stream >> id2;
                    file_stream >> height;
                    file_stream >> r;
                    file_stream >> g;
                    file_stream >> b;

                    // Add the wall to the map
                    AddWall(id1, id2, height, r, g, b);

                    file_stream >> cur_line_header;
                }
            }
            catch(...) {
                std::cout << "FATAL ERROR: Unable to load file '" << file_name << "'(Incorrect format?)";
            }
        }

        std::vector<Wall> walls;
        std::vector<Vertex> vertices;
};

#endif