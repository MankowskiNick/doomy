
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
#include "geom.h"

#define WIDTH_DRAW_SCALAR 200 / tan(FOV)
#define HEIGHT_DRAW_SCALAR WIDTH_DRAW_SCALAR * (float)WIDTH / (float)HEIGHT

// TODO: Abstract some of the functionality of these away to geom.h

// Perform a translation on the walls to make the illusion that the camera is moving
void ShiftWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        Vertex camera_pos = {
            .x = camera.x,
            .y = camera.y,
            .z = camera.z
        };

        Translate(map.walls[i].line.v1, camera_pos, -1.0f);
        Translate(map.walls[i].line.v2, camera_pos, -1.0f);
        
    }
}

// Rotate the vertices on the map around the camera to make the illusion the camera is turning
void RotateWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        Rotate(map.walls[i].line.v1, camera.angle);
        Rotate(map.walls[i].line.v2, camera.angle);
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
        int x1 = (int)(map.walls[i].line.v1.x * MINIMAP_SCALE) + (WIDTH / 2);
        int y1 = (int)(map.walls[i].line.v1.y * MINIMAP_SCALE) + (HEIGHT / 2);

        int x2 = (int)(map.walls[i].line.v2.x * MINIMAP_SCALE) + (WIDTH / 2);
        int y2 = (int)(map.walls[i].line.v2.y * MINIMAP_SCALE) + (HEIGHT / 2);

        DrawLine(x1, y1, x2, y2, map.walls[i].color[0], map.walls[i].color[1], map.walls[i].color[2]);
    }
    DrawPlayer(camera);
}

// Check if the wall needs rendered
bool IsInFront(Wall& wall, Camera& camera) {
    return wall.line.v1.y > 0 ||  wall.line.v2.y > 0;
}


// Calculate what column the vertex will be at
int MapToScreenX(const Vertex& vert, Camera& camera) {
    float y = (vert.y == 0) ? 1.0f : vert.y; // Prevent division by 0
    return (WIDTH_DRAW_SCALAR * vert.x / y) + (WIDTH / 2);
}

// Calculate what row the vertex will be at
int MapToScreenY(const Vertex& vert, Camera& camera) {
    float y = (vert.y == 0) ? 1.0f : vert.y; // Prevent division by 0
    return (HEIGHT_DRAW_SCALAR * vert.z / y) + (HEIGHT / 2);
}

int MapToScreenHeight(const Vertex& vert, Camera& camera, float wall_height) {
    Vertex second_vert = {
        .x = vert.x,
        .y = vert.y,
        .z = vert.z + wall_height
    };
    return MapToScreenY(second_vert, camera) - MapToScreenY(vert, camera);
}

void ClipWall(Vertex& neg_y_vert, const Vertex& other) { // TODO: maybe refactor parameters?
    float dx = other.x - neg_y_vert.x;
    float dy = other.y - neg_y_vert.y;
    float dz = other.z - neg_y_vert.z;

    if (dy == 0.0f)
        dy = 1.0f; // prevent division by 0

    float scalar = -1.0f * neg_y_vert.y / dy;
    float new_x = neg_y_vert.x + scalar * dx;
    float new_z = neg_y_vert.z + scalar * dz;

    neg_y_vert.x = new_x;
    neg_y_vert.y = 0.01f;
    neg_y_vert.z = new_z;

} 

// Draw an individual wall
void DrawWall(Wall& wall, Map& map, Camera& camera) {

    if (IsInFront(wall, camera)) {

        // Fix a graphical bug that occurs when one point is behind the player
        if (wall.line.v1.y < 0)
            ClipWall(wall.line.v1, wall.line.v2);
        if (wall.line.v2.y < 0)
            ClipWall(wall.line.v2, wall.line.v1);

        // Calculate info for each vertex in the wall
        int screencol_1 = MapToScreenX(wall.line.v1, camera);
        int screencol_2 = MapToScreenX(wall.line.v2, camera);

        int vert1_bottom_y = MapToScreenY(wall.line.v1, camera);
        int vert2_bottom_y = MapToScreenY(wall.line.v2, camera); 

        int vert1_height = MapToScreenHeight(wall.line.v1, camera, wall.height);
        int vert2_height = MapToScreenHeight(wall.line.v2, camera, wall.height);

        float wallheight_stepsize = (float)(vert2_height - vert1_height) / (float)(screencol_2 - screencol_1);
        float screenrow_stepsize = (float)(vert2_bottom_y - vert1_bottom_y) / (float)(screencol_2 - screencol_1);

        int step = screencol_1 < screencol_2 ? 1 : -1;

        // Make sure we aren't drawing what isn't on the screen, even if it isn't drawn it will still take compute time to loop through.
        int start_col = (screencol_1 < 0) ? 0 : ((screencol_1 > WIDTH - 1) ? WIDTH - 1 : screencol_1);
        int end_col = (screencol_2 < 0) ? 0 : ((screencol_2 > WIDTH - 1) ? WIDTH - 1 : screencol_2);

        for (int col = start_col; col != end_col; col += step) {
            if (col < 0)
                col = 0;
            if (col >= WIDTH)
                col = WIDTH - 1;
            int cur_wallheight = (int)(vert1_height + (wallheight_stepsize * (col - screencol_1)));
            int cur_screenrow = (int)(vert1_bottom_y + (screenrow_stepsize * (col - screencol_1)));

            DrawLineVert(col, cur_screenrow, cur_screenrow + cur_wallheight, wall.color);
        }
    }
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
    //DrawMinimap(map, camera);
    DrawWalls(map, camera);
}
