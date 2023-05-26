
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

void InitView(GLLib& gl);

// Draw function
void Draw(GLLib& gl);

void DrawPixel(int x, int y, int r, int g, int b);

void DrawPlayer(Camera& camera);

// Draw a single vertical line
void DrawLineVert(int col, int bot_row, int top_row, int color[3]);

void DrawLine(int x1, int y1, int x2, int y2, int r, int g, int b);

void FillScreen(int r, int g, int b);