#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <Glimpse/glimpse.h>
#include <Glimpse/consoleout.h>
#include <Glimpse/status.h>
#include <Glimpse/glerrorcode.h>

#include "bsp_tree.h"
#include "common_struct.h"

#define MAX_LINE_COUNT 10000

// TODO: Add more error handling here

class Map {
    public:
        
        Map(Glimpse::GlimpseLogger* newLogger) {
            logger = newLogger;
        }

        Map(Map* copy_map, Glimpse::GlimpseLogger* logger) {
            for (int i =0; i < copy_map->vertices.size(); i++)
                AddVertex(copy_map->vertices[i]);

            for (int i = 0; i < copy_map->walls.size(); i++)
                AddWall(copy_map->walls[i]);

                bsp_tree = copy_map->bsp_tree;
        }
        
        ~Map() {}

        void AddVertex(int id, float x, float y) {
            Vertex vert = {
                .id = id,
                .x = x,
                .y = y
            };

            vertices.push_back(vert);
        }

        void AddVertex(const Vertex& vert) {
            AddVertex(vert.id, vert.x, vert.y);
        }

        void AddWall(int wall_id, 
            int id1, int id2, 
            float wall_min, float wall_max, 
            float floor_height, float ceiling_height, 
            int r, int g, int b) {
            Wall wall;
            for (int i = 0; i < vertices.size(); i++) {
                if (vertices[i].id == id1) wall.line.v1 = vertices[i];
                if (vertices[i].id == id2) wall.line.v2 = vertices[i];
            }
            wall.id = wall_id;
            wall.min_height = wall_min;
            wall.max_height = wall_max;
            wall.floor_height = floor_height;
            wall.ceiling_height = ceiling_height;
            wall.color[0] = r;
            wall.color[1] = g;
            wall.color[2] = b;
            wall.is_portal = floor_height != -1 && ceiling_height != -1;
            walls.push_back(wall);
        }

        void AddWall(const Wall& wall) {
            AddWall(wall.id, 
                    wall.line.v1.id, wall.line.v2.id, 
                    wall.min_height, wall.max_height, 
                    wall.floor_height, wall.ceiling_height, 
                    wall.color[0], wall.color[1], wall.color[2]);
        }

        void LoadFile(std::string file_name) {
            // File format: 
            //     [verts]
            //     v1.id v1.x v1.y is_temp
            //     ...
            //     [walls]
            //     id v1.id v2.id wall_min wall_max floor_min floor_max r g b is_temp is_ancestral
            //     ...
            //     [bsp]
            //     bsp string
            //     [end]
            try {
                std::ifstream file_stream;

                file_stream.open(file_name);

                if (!file_stream.is_open()) {
                    logger->Log("Unable to open file '" + file_name + "'. Check file path and permissions.", Glimpse::FATAL);
                    return;
                }

                if (!file_stream.good()) {
                    logger->Log("File stream is not good after opening file '" + file_name + "'. Check the file contents and encoding.", Glimpse::FATAL);
                    return;
                }

                std::string cur_line_header = "";

                int skiplines = 0;
                while (cur_line_header != "[verts]") {
                    skiplines++;
                    if (skiplines > MAX_LINE_COUNT) 
                        logger->Log("Invalid level file(" + file_name + ").", Glimpse::FATAL);
                    file_stream >> cur_line_header;
                }
                file_stream >> cur_line_header;

                while (cur_line_header != "[walls]") {

                    // Declare variables needed
                    int id, is_temp;
                    float x, y;

                    // Input data values
                    file_stream >> id;
                    file_stream >> x;
                    file_stream >> y;
                    file_stream >> is_temp;

                    // Add the vertex to the map
                    AddVertex(id, x, y);

                    file_stream >> cur_line_header;
                }

                file_stream >> cur_line_header;

                while (cur_line_header != "[bsp]") {

                    // Declare variables needed
                    int wall_id, id1, id2, is_temp, is_ancestral; 
                    float wall_min, wall_max;
                    float floor_height, ceiling_height;
                    int r, g, b;;

                    // Input the data values
                    file_stream >> wall_id;
                    file_stream >> id1;
                    file_stream >> id2;
                    file_stream >> wall_min;
                    file_stream >> wall_max;
                    file_stream >> floor_height;
                    file_stream >> ceiling_height;
                    file_stream >> r;
                    file_stream >> g;
                    file_stream >> b;
                    file_stream >> is_temp;
                    file_stream >> is_ancestral;

                    // Add the wall to the map
                    if (is_ancestral == 0)
                        AddWall(wall_id, 
                                id1, id2, 
                                wall_min, wall_max, 
                                floor_height, ceiling_height, 
                                r, g, b);

                    file_stream >> cur_line_header;
                }

                std::string bsp_string = "";
                file_stream >> bsp_string;
                bsp_tree = DeseralizeBSP(bsp_string);
            }
            catch(...) {
                logger->Log("FATAL ERROR: Unable to load file '" + file_name + "'(Incorrect format?)", Glimpse::FATAL);
            }
        }


        Wall* GetWallByID(int wall_id) {
            for (int i = 0; i < walls.size(); i++)
                if (walls[i].id == wall_id) 
                    return &walls[i];
            return NULL;
        }

        BSP_Tree bsp_tree;
        std::vector<Wall> walls;
        std::vector<Vertex> vertices;

        Glimpse::GlimpseLogger* logger;
};

#endif