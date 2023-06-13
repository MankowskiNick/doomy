
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

struct Point {
    float x, y, z;
};

// Define the cube mesh by specifying the order of connecting points
int cubeIndices[36] = {
    0, 2, 1,  // Front face
    1, 2, 3,
    1, 3, 5,  // Right face
    5, 3, 7,
    5, 7, 4,  // Back face
    4, 7, 6,
    4, 6, 0,  // Left face
    0, 6, 2,
    2, 6, 3,  // Top face
    3, 6, 7,
    0, 1, 4,  // Bottom face
    4, 1, 5
};

Point cube[8] = {
    {1.5f, -0.5f, -0.5f},   // Point 1
    {2.5f, -0.5f, -0.5f},   // Point 2
    {1.5f, 0.5f, -0.5f},    // Point 3
    {2.5f, 0.5f, -0.5f},    // Point 4
    {1.5f, -0.5f, 0.5f},    // Point 5
    {2.5f, -0.5f, 0.5f},    // Point 6
    {1.5f, 0.5f, 0.5f},     // Point 7
    {2.5f, 0.5f, 0.5f}      // Point 8
};

void rotatePointXZ(Point& point, float r) {
    //Point rotatedPoint;

    point.x = point.x * cos(r) + point.z * sin(r);
    point.y = point.y;
    point.z = -point.x * sin(r) + point.z * cos(r);
}

// Render code
void Render(Map map, Camera& camera, int frameCount) {

    float SCALAR = 50.0f;
    // Display a gray background
    FillScreen(100, 100, 100);
    float last_screenx, last_screeny;

    float rads = 0.001;

    for (int j = 0; j < 36; j++) {

        int i = cubeIndices[j];
        float screenx, screeny;

        //rotatePointXZ(cube[i], rads);
        
        screenx = SCALAR * cube[i].x / cube[i].y;
        screeny = SCALAR * cube[i].z / cube[i].y;

        DrawLine(screenx + (0.5 * WIDTH), screeny + (0.5 * HEIGHT), last_screenx + (0.5 * WIDTH), last_screeny + (0.5 * HEIGHT), 255, 0, 0);


        //DrawPixel(screenx + (0.5 * WIDTH), screeny + (0.5 * HEIGHT), 255, 0, 0);
        last_screenx = screenx;
        last_screeny = screeny;
    }

    // Draw walls
    //TranslateMap(map, camera);
    //DrawMinimap(map, camera);
}
