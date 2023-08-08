#ifndef STD_GRAPHICS_H
#define STD_GRAPHICS_H

#define BLACK 0, 0, 0
#define WHITE 255, 255, 255

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

class StdGraphicsHandler {
    public:
        StdGraphicsHandler(GLLib& gl);

        ~StdGraphicsHandler();

        // Paint to screen
        void UpdateDisplay();

        // Draw a single pixel
        void DrawPixel(int x, int y, int r, int g, int b);

        // Draw a single vertical line
        void DrawLineVert(int col, int bot_row, int top_row, int color[3]);

        // Draw a line between (x1,y1) and (x2,y2)
        void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);

        // Draw a circle centered at (x,y) w/ radius radius and color (r,g,b)
        void DrawCircle(int x, int y, int radius, int r, int g, int b);

        // Fill screen with color (r,g,b)
        void FillScreen(int r, int g, int b);

    private:
        // GL Library Instance
        GLLib* gl;

        // Pixel data
        unsigned char pixelData[HEIGHT][WIDTH][3];
};

#endif