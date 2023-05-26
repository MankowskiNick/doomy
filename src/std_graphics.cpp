
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "shared_graphics.h"

unsigned char pixelData[HEIGHT][WIDTH][3];

// Buffer view coords to GPU
void InitView(GLLib& gl) {
    float view[] = {
        -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f,

        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,        1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f
    };
    gl.BufferVerticesWithTextureCoords(view, sizeof(view));
}

// Draw function
void Draw(GLLib& gl) {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create a Texture object and bind it to texture unit 1
    Texture screenView;
    screenView.LoadData(&pixelData[0][0][0], WIDTH, HEIGHT, &gl, 0, GL_TEXTURE1, "pixels");


    // Activate the texture unit and bind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, screenView.GetTextureID());

    // Draw a rectangle occupying the whole screen with that texture
    gl.DrawTriangles(6);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawPixel(int x, int y, int r, int g, int b) {
    if (x < WIDTH && x >= 0 && y < HEIGHT && y >= 0) {
        pixelData[y][x][0] = r;
        pixelData[y][x][1] = g;
        pixelData[y][x][2] = b;
    }
}

void DrawPlayer(Camera& camera) {
    for (int x = (WIDTH / 2) - 5; x < (WIDTH / 2) + 5; x++) {
        for (int y = (HEIGHT / 2) - 5; y < (HEIGHT / 2) + 5; y++) {
            DrawPixel(x, y, 255, 0, 0);
        }
    }
    for (int i = (WIDTH / 2 ) + 5; i < (WIDTH / 2) + 10; i++) {
        DrawPixel(i, HEIGHT / 2, 255, 0, 0);
    }

    float slope = tan(camera.fov / 2);
    for (int x = WIDTH / 2; x < WIDTH; x++) {
        DrawPixel(x, (HEIGHT / 2) + ((x - WIDTH / 2) * slope), 255, 255, 255);
        DrawPixel(x, (HEIGHT / 2) - ((x - WIDTH / 2) * slope), 255, 255, 255);
    }
}

// Draw a single vertical line
void DrawLineVert(int col, int bot_row, int top_row, int color[3]) {
    for (int i = bot_row; i <= top_row; i++) {
        DrawPixel(col, i, color[0], color[1], color[2]);
    }
}

void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b) {
    float dx = (float)(x2 - x1) / 300;
    float dy = (float)(y2 - y1) / 300;
    float cur_x = x1;
    float cur_y = y1;
    for (int i = 0; i < 300; i++) {
        cur_x += dx;
        cur_y += dy;
        DrawPixel((int)cur_x, (int)cur_y, r, g, b);
    }
}


void FillScreen(int r, int g, int b) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            DrawPixel(i, j, r, g, b);
        }
    }
}