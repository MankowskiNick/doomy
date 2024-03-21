#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include <Glimpse/glimpse.h>
#include <Glimpse/consoleout.h>
#include <Glimpse/status.h>
#include <Glimpse/glerrorcode.h>

#include "bsp_tree.h"
#include "common_struct.h"

#define MAX_LINE_COUNT 10000

// TODO: 
//      -Add more error handling here

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
            int r, int g, int b
        ) {
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
            std::ifstream file;
            file.open(file_name);

            // Handle errors
            if (!file.is_open()) {
                logger->Log("Unable to open file '" + file_name + "'. Check file path and permissions.", Glimpse::FATAL);
                return;
            }
            if (!file.good()) {
                logger->Log("File stream is not good after opening file '" + file_name + "'. Check the file contents and encoding.", Glimpse::FATAL);
                return;
            }

            // Parse the file by section
            std::string line, section;
            while (getline(file, line)) {
                // Get the section if a new section is present
                if (line == "[verts]")
                    section = "verts";
                else if (line == "[walls]")
                    section = "walls";
                else if (line == "[sectors]")
                    section = "sectors";
                else if (line == "[bsp]")
                    section = "bsp";
                else if (line == "[end]")
                    return;

                // Handle line if it is parsing data
                else
                    HandleLine(section, line);
            }
        }


        Wall* GetWallById(int wall_id) {
            for (int i = 0; i < walls.size(); i++)
                if (walls[i].id == wall_id) 
                    return &walls[i];
            return NULL;
        }

        Subsector* GetSectorById(int id) {
            for (int i = 0; i < sectors.size(); i++)
                if (sectors[i].id == id) 
                    return &sectors[i];
            return NULL;
        }

        // TODO: Are these private?
        BSP_Tree bsp_tree;
        std::vector<Wall> walls;
        std::vector<Vertex> vertices;
        std::vector<Subsector> sectors;

        Glimpse::GlimpseLogger* logger;
private:
    // Parse data coming from a file based on the section
    void HandleLine(const std::string& section, const std::string& text) {
        if (section == "verts")
            ParseVertex(text);
        else if (section == "walls")
            ParseWall(text);
        else if (section == "sectors")
            ParseSector(text);
        else if (section == "bsp")
            ParseBSPString(text);
        else
            logger->Log("FATAL ERROR: Invalid BSP file section.", Glimpse::FATAL);
    }

    // Parse vertex line
    void ParseVertex(const std::string& text) {
        // TODO: Handle errors better, maybe use try/catch
        if (text.rfind("vert:", 0) == 0) { // Check if the string starts with "vert:"
            std::istringstream iss(text);
            std::string token;
            int id;
            float x, y;
            iss >> token >> id >> x >> y; // Parse the line
            if (iss) { // Check if parsing was successful
                this->AddVertex(id, x, y);
            } 
            else {
                logger->Log("FATAL ERROR: Error parsing map vertex file.(Incorrect format?)", Glimpse::FATAL);
            }
        }
    }

    void ParseWall(const std::string& text) {
        if (text.rfind("wall:", 0) == 0) { // Check if the string starts with "wall:"
            std::istringstream iss(text);
            std::string token;
            int wall_id, v1_id, v2_id;
            float wall_min, wall_max, floor_height, ceiling_height;
            int r, g, b;
            iss >> token >> wall_id >> v1_id >> v2_id >> wall_min >> wall_max >> floor_height >> ceiling_height >> r >> g >> b;
            if (iss) { // Check if parsing was successful
                this->AddWall(wall_id, 
                                v1_id, v2_id,  
                                wall_min, wall_max, 
                                floor_height, ceiling_height, 
                                r, g, b);
            } 
            else {
                logger->Log("FATAL ERROR: Error parsing wall in map file.(Incorrect format?)", Glimpse::FATAL);
            }
        }
    }

    void ParseSector(const std::string& text) {
        if (text.rfind("sector:", 0) == 0) { // Check if the string starts with "sector:"
            std::istringstream iss(text);

            std::string token;
            int id, wall_id;

            Subsector subsect;

            iss >> token >> id;
            subsect.id = id;

            while (iss >> wall_id) {
                Wall* cur_wall = this->GetWallById(wall_id);
                subsect.walls.push_back(cur_wall);
            }

            // add subsector to map
            sectors.push_back(subsect);
        }
        else {
            logger->Log("FATAL ERROR: Error parsing sector in map file.(Incorrect format?)", Glimpse::FATAL);
        }
    }

    void ParseBSPString(const std::string& text) {
        bsp_tree = DeseralizeBSP(text);
    }

};

#endif