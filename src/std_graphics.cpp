
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>

#include "constants.h"
#include "std_graphics.h"
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "shared_graphics.h"

// Buffer view coords to GPU
StdGraphicsHandler::StdGraphicsHandler(GLLib& newGL) {
    float view[] = {
        -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f,

        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,        1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f,       0.0f, 1.0f
    };

    gl = &newGL;

    gl->BindShader("shaders/shader.vsh", "shaders/shader.fsh");
    gl->BufferVerticesWithTextureCoords(view, sizeof(view));
}

StdGraphicsHandler::~StdGraphicsHandler() {
    glfwDestroyWindow(gl->GetWindow());
    glfwTerminate();
 }

// Display function
// Send what we have drawn to the screen
void StdGraphicsHandler::UpdateDisplay() {

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Create a Texture object and bind it to texture unit 1
    Texture screenView;
    screenView.LoadData(&pixelData[0][0][0], WIDTH, HEIGHT, gl, 0, GL_TEXTURE1, "pixels");


    // Activate the texture unit and bind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, screenView.GetTextureID());

    // Draw a rectangle occupying the whole screen with that texture
    gl->DrawTriangles(6);

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glfwSwapBuffers(gl->GetWindow());
}

void StdGraphicsHandler::DrawPixel(int x, int y, int r, int g, int b) {
    if (x < WIDTH && x >= 0 && y < HEIGHT && y >= 0) {
        pixelData[y][x][0] = r;
        pixelData[y][x][1] = g;
        pixelData[y][x][2] = b;
    }
}

// Draw a single vertical line
void StdGraphicsHandler::DrawLineVert(int col, int bot_row, int top_row, int color[3]) {
    if (bot_row < 0)
        bot_row = 0;
    if (bot_row >= WIDTH)
        bot_row = WIDTH - 1;
    if (top_row < 0)
        top_row = 0;
    if (top_row >= WIDTH)
        top_row = WIDTH - 1;
    for (int i = bot_row; i <= top_row; i++) {
        DrawPixel(col, i, color[0], color[1], color[2]);
    }
}

void StdGraphicsHandler::DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b) {
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


void StdGraphicsHandler::FillScreen(int r, int g, int b) {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            DrawPixel(i, j, r, g, b);
        }
    }
}


void StdGraphicsHandler::DrawCircle(int x, int y, int radius, int r, int g, int b) {
    for (int i = x - radius; i <= x + radius; i++) {
        for (int j = y - radius; j <= y + radius; j++) {
            int dx = i - x;
            int dy = j - y;
            int distanceSquared = dx * dx + dy * dy;

            if (distanceSquared <= radius * radius) // Check if the point is within the circle
                DrawPixel(i, j, r, g, b);
        }
    }
}