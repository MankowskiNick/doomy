#ifndef SCREEN_SPACE_MAPPER_H
#define SCREEN_SPACE_MAPPER_H

#include "common_struct.h"

#define WIDTH_DRAW_SCALAR 200 / tan(FOV)
#define HEIGHT_DRAW_SCALAR WIDTH_DRAW_SCALAR * (float)WIDTH / (float)HEIGHT

// TODO: Do we want to make this a class?

// Calculate what column the vertex will be at
int MapToScreenX(const Vertex& vert);

// Calculate what row the vertex will be at
int MapToScreenY(const Vertex& vert);

int MapToScreenHeight(const Vertex& vert, float wall_height);

void ClipWall(Vertex& neg_y_vert, const Vertex& other);

#endif