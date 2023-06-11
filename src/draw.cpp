
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


// TODO: Abstract some of the functionality of these away to geom.h

// Perform a translation on the walls to make the illusion that the camera is moving
void ShiftWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        map.walls[i].line.v1.x -= camera.x;
        map.walls[i].line.v1.y -= camera.y;

        map.walls[i].line.v2.x -= camera.x;
        map.walls[i].line.v2.y -= camera.y;
    }
}

// Rotate the vertices on the map around the camera to make the illusion the camera is turning
void RotateWalls(Map& map, Camera& camera) {
    for (int i = 0; i < map.walls.size(); i++) {
        float x1 = map.walls[i].line.v1.x;
        float y1 = map.walls[i].line.v1.y;
        float new_x1 = x1 * cos(-1 * camera.angle) - y1 * sin(-1 * camera.angle);
        float new_y1 = y1 * cos(-1 * camera.angle) + x1 * sin(-1 * camera.angle);
        map.walls[i].line.v1.x = new_x1;
        map.walls[i].line.v1.y = new_y1;

        float x2 = map.walls[i].line.v2.x;
        float y2 = map.walls[i].line.v2.y;
        float new_x2 = x2 * cos(-1 * camera.angle) - y2 * sin(-1 * camera.angle);
        float new_y2 = y2 * cos(-1 * camera.angle) + x2 * sin(-1 * camera.angle);
        map.walls[i].line.v2.x = new_x2;
        map.walls[i].line.v2.y = new_y2;
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

// Calculate what vertex from the bottom the wall will be at
int CalcRowOffset(const Vertex& vert, Camera& camera, int wallheight) {
    float x = vert.x;
    float y = vert.y;
    int screenrow = (int)(((HEIGHT - wallheight) / 2) + (camera.z / sqrt(pow(x, 2) + pow(y, 2)))); // may not use y, may use sqrt(pow(x, 2) + pow(y, 2))
    return screenrow;
}

// Calculate the wall height for a given wall
int CalcWallHeight(const Wall& wall, const Vertex& vert, Camera& camera) {
    float x = abs(vert.x);
    float y = vert.y;
    float dist = sqrt(pow(x, 2) + pow(y, 2));
    if (x > 0)
        return (int)(WALLHEIGHT_SCALAR * ((wall.h_ceiling - wall.h_floor) / dist));
    else
        return 0;
}

// Calculate what column the vertex will be at
int CalcColumnOffset(const Vertex& vert, Camera& camera) {
    float x = vert.x;
    float y = vert.y;

    float angle_to_vert = atan2(y, x);
    float angle_in_degs = angle_to_vert * 180.0f / (M_PI * 2); // TEMP DEBUG

    float perc = angle_to_vert / (camera.fov / 2.0f);

    int col = (perc * (WIDTH / 2)) + (WIDTH / 2);

    return col;
}

// Check which side the the column should appear on.  We need to check this because
// of the wall rendering in the wrong direction.
int AdjustColumn(Wall wall) {
    Line detector_line_up = {
        .v1 = {
            .x = 0,
            .y = 0,
        },
        .v2 = {
            .x = 0,
            .y = RENDER_DISTANCE,
        }
    };    
    Line detector_line_down = {
        .v1 = {
            .x = 0,
            .y = 0,
        },
        .v2 = {
            .x = 0,
            .y = -1 * RENDER_DISTANCE,
        }
    };

    return Intersect(wall.line, detector_line_up) ? WIDTH - 1 : 0;
}

bool IsInFront(Wall& wall, Camera& camera) {
    
    // Check if either point is in the fov and in front.  If so, display it.
    float fov_slope = tan(camera.fov / 2);
    float v1_slope = wall.line.v1.y / wall.line.v1.x;
    float v2_slope = wall.line.v2.y / wall.line.v2.x;

    bool v1_in = abs(v1_slope) <= abs(fov_slope) && wall.line.v1.x > 0;
    bool v2_in = abs(v2_slope) <= abs(fov_slope) && wall.line.v2.x > 0;

    // We still need to consider the case where a wall is very close to the camera and the vertices are out of the fov.  In this case, this wall would be "invisible"
    // according to the fov check method, so let's check to see if the line produced intersects with the fov lines.

    // Create fov lines    
    Line fov_line_top = {
        .v1 = {
            .x = 0.0f,
            .y = 0.0f,
        },
        .v2 = {
            .x = RENDER_DISTANCE,
            .y = fov_slope * RENDER_DISTANCE
        }
    };
    Line fov_line_bot = {
        .v1 = {
            .x = 0.0f,
            .y = 0.0f,
        },
        .v2 = {
            .x = RENDER_DISTANCE,
            .y = -1.0f * fov_slope * RENDER_DISTANCE
        }
    };

    bool intersects_fov_lines = Intersect(fov_line_top, wall.line) || Intersect(fov_line_bot, wall.line);

    return (v1_in || v2_in) || intersects_fov_lines;
}

// Draw an individual wall
void DrawWall(Wall& wall, Map& map, Camera& camera) {

    if (IsInFront(wall, camera)) {

        // Calculate info for each vertex in the wall
        int screencol_1 = CalcColumnOffset(wall.line.v1, camera);
        int screencol_2 = CalcColumnOffset(wall.line.v2, camera);

        if (wall.line.v1.x < 0)
            screencol_1 = AdjustColumn(wall);
        if (wall.line.v2.x < 0)
            screencol_2 = AdjustColumn(wall);

        int wallheight_1 = CalcWallHeight(wall, wall.line.v1, camera);
        int wallheight_2 = CalcWallHeight(wall, wall.line.v2, camera);

        int screenrow_1 = 100;//CalcRowOffset(wall.line.v1, camera, wallheight_1);
        int screenrow_2 = 100;//CalcRowOffset(wall.line.v2, camera, wallheight_2);

        // Calculate the step size for wallheight
        float wallheight_stepsize = (float)(wallheight_2 - wallheight_1) / (float)(screencol_2 - screencol_1);
        float screenrow_stepsize = (float)(screenrow_2 - screenrow_1) / (float)(screencol_2 - screencol_1);

        // Don't allow drawing past the edge of the screen.
        if (screencol_1 < 0)
            screencol_1 = 0;
        if (screencol_1 >= WIDTH)
            screencol_1 = WIDTH - 1;
        if (screencol_2 < 0)
            screencol_2 = 0;
        if (screencol_2 >= WIDTH)
            screencol_2 = WIDTH - 1;

        for (int col = fmin(screencol_1, screencol_2); col < fmax(screencol_1, screencol_2); col++) {
            int cur_wallheight = (int)(wallheight_1 + (wallheight_stepsize * (col)));
            int cur_screenrow = (int)(screenrow_1 + (screenrow_stepsize * (col)));

            DrawLineVert(col, cur_screenrow, cur_screenrow + cur_wallheight, wall.color);
        }

        std::cout << "wall.color=[" << wall.color[0] << "," << wall.color[1] << "," << wall.color[2] << "], screencol_1=" << screencol_1 << ", screencol_2=" << screencol_2 << "\n";
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
    DrawWalls(map, camera);
    DrawMinimap(map, camera);
}
