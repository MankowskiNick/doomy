
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>

#include "shared_graphics.h"

#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "std_graphics.h"

// Perform a translation on the walls to make the illusion that the camera is moving
void ShiftWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        map.walls[i].verts[0].coords[0] -= camera.x;
        map.walls[i].verts[0].coords[1] -= camera.y;

        map.walls[i].verts[1].coords[0] -= camera.x;
        map.walls[i].verts[1].coords[1] -= camera.y;
    }
}

// Rotate the vertices on the map around the camera to make the illusion the camera is turning
void RotateWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        float x1 = map.walls[i].verts[0].coords[0];
        float y1 = map.walls[i].verts[0].coords[1];
        float new_x1 = x1 * cos(-1 * camera.angle) - y1 * sin(-1 * camera.angle);
        float new_y1 = y1 * cos(-1 * camera.angle) + x1 * sin(-1 * camera.angle);
        map.walls[i].verts[0].coords[0] = new_x1;
        map.walls[i].verts[0].coords[1] = new_y1;

        float x2 = map.walls[i].verts[1].coords[0];
        float y2 = map.walls[i].verts[1].coords[1];
        float new_x2 = x2 * cos(-1 * camera.angle) - y2 * sin(-1 * camera.angle);
        float new_y2 = y2 * cos(-1 * camera.angle) + x2 * sin(-1 * camera.angle);
        map.walls[i].verts[1].coords[0] = new_x2;
        map.walls[i].verts[1].coords[1] = new_y2;
    }
}

// Perform translations necessary to create the illusion of looking different directions & moving
void TranslateMap(Map& map, Camera& camera) {
    ShiftWalls(map, camera);
    RotateWalls(map, camera);
}

// Draw the minimap
void DrawMinimap(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        int x1 = (int)(map.walls[i].verts[0].coords[0] * MINIMAP_SCALE) + (WIDTH / 2);
        int y1 = (int)(map.walls[i].verts[0].coords[1] * MINIMAP_SCALE) + (HEIGHT / 2);

        int x2 = (int)(map.walls[i].verts[1].coords[0] * MINIMAP_SCALE) + (WIDTH / 2);
        int y2 = (int)(map.walls[i].verts[1].coords[1] * MINIMAP_SCALE) + (HEIGHT / 2);

        DrawLine(x1, y1, x2, y2, map.walls[i].color[0], map.walls[i].color[1], map.walls[i].color[2]);
    }
    DrawPlayer(camera);
}

// Calculate what column the vertex will be at
int CalcColumnOffset(const Vertex& vert, Camera& camera) {
    int screencol = (int)(((vert.coords[1] / vert.coords[0]) / tan(camera.fov / 2)) * WIDTH / 2)+ (WIDTH / 2);
    return screencol;
}

// Calculate what vertex from the bottom the wall will be at
int CalcRowOffset(const Vertex& vert, Camera& camera, int wallheight) {
    float x = vert.coords[0];
    float y = vert.coords[1];
    int screenrow = (int)(((HEIGHT - wallheight) / 2) + (camera.z / sqrt(pow(x, 2) + pow(y, 2)))); // may not use y, may use sqrt(pow(x, 2) + pow(y, 2))
    return screenrow;
}

bool IsInFront(Wall& wall, Camera& camera) {
    float fov_slope = tan(camera.fov / 2);
    float v1_slope = wall.verts[0].coords[1] / wall.verts[0].coords[0];
    float v2_slope = wall.verts[1].coords[1] / wall.verts[1].coords[0];

    bool within_fov = (abs(v1_slope) <= abs(fov_slope)) || (abs(v2_slope) <= abs(fov_slope));
    bool in_front = (wall.verts[0].coords[0] > 0) || (wall.verts[1].coords[0] > 0);

    return in_front && within_fov;
}

// Calculate the wall height for a given wall
int CalcWallHeight(const Wall& wall, const Vertex& vert, Camera& camera) {
    float x = vert.coords[0];
    float y = vert.coords[1];
    float dist = sqrt(pow(x, 2) + pow(y, 2));
    if (x > 0)
        return (int)(WALLHEIGHT_SCALAR * ((wall.h_ceiling - wall.h_floor) / dist));
    else
        return 0;
}

// Draw an individual wall
void DrawWall(Wall& wall, Map& map, Camera& camera) {

    // Calculate info for each vertex in the wall
    int screencol_1 = CalcColumnOffset(wall.verts[0], camera);
    int screencol_2 = CalcColumnOffset(wall.verts[1], camera);
    DrawLineVert(screencol_1, 0, 100, wall.color);
    DrawLineVert(screencol_2 - 1, 0, 100, wall.color);

    int wallheight_1 = 100;//CalcWallHeight(wall, wall.verts[0], camera);
    int wallheight_2 = 100;//CalcWallHeight(wall, wall.verts[1], camera);

    int screenrow_1 = 100;//CalcRowOffset(wall.verts[0], camera, wallheight_1);
    int screenrow_2 = 100;//CalcRowOffset(wall.verts[1], camera, wallheight_2);

    // Calculate the step size for wallheight
    float wallheight_stepsize = (float)(wallheight_2 - wallheight_1) / (float)(screencol_2 - screencol_1);
    float screenrow_stepsize = (float)(screenrow_2 - screenrow_1) / (float)(screencol_2 - screencol_1);
    if (IsInFront(wall, camera)) {
        // int step = (screencol_1 < screencol_2) ? 1 : -1;
        // if (screencol_1 < 0) 
        //     screencol_1 = 0;
        // if (screencol_1 >= WIDTH)
        //     screencol_1 = WIDTH - 1;
        for (int col = fmin(screencol_1, screencol_2); col < fmax(screencol_1, screencol_2) && col < WIDTH && col >= 0; col ++) {
            int cur_wallheight = (int)(wallheight_1 + (wallheight_stepsize * (col)));
            int cur_screenrow = (int)(screenrow_1 + (screenrow_stepsize * (col)));

            DrawLineVert(col, cur_screenrow, cur_screenrow + cur_wallheight, wall.color);
        }
    }

    std::cout << "wall.color=[" << wall.color[0] << "," << wall.color[1] << "," << wall.color[2] << "], screencol_1=" << screencol_1 << ", screencol_2=" << screencol_2 << "\n";
}

// Draw all walls
void DrawWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        DrawWall(map.walls[i], map, camera);
    }
}

// Render code
void Render(Map map, Camera& camera) {

    // Display a gray background
    FillScreen(100, 100, 100);

    // Draw walls
    TranslateMap(map, camera);
    DrawWalls(map, camera);
    DrawMinimap(map, camera);
}
