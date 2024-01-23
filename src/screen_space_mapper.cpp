
#include <Glaze/std_graphics.h>

#include "screen_space_mapper.h"
#include "common_struct.h"

// Calculate what column the vertex will be at
int MapToScreenX(const Vertex& vert) {
    float y = (abs(vert.y) < ERROR_MARGIN) ? 10000.0f : vert.y; // Prevent division by 0
    return (WIDTH_DRAW_SCALAR * vert.x / y) + (WIDTH / 2);
}

// Calculate what row the vertex will be at
int MapToScreenY(const Vertex& vert) {
    float y = (abs(vert.y) < ERROR_MARGIN) ? 10000.0f : vert.y; // Prevent division by 0
    return (HEIGHT_DRAW_SCALAR * vert.z / y) + (HEIGHT / 2);
}

int MapToScreenHeight(const Vertex& vert, float wall_height) {
    Vertex second_vert = {
        .x = vert.x,
        .y = vert.y,
        .z = vert.z + wall_height
    };
    return MapToScreenY(second_vert) - MapToScreenY(vert);
}

void ClipWall(Vertex& neg_y_vert, const Vertex& other) {
    float dx = other.x - neg_y_vert.x;
    float dy = other.y - neg_y_vert.y;
    float dz = other.z - neg_y_vert.z;

    if (abs(dy) < ERROR_MARGIN)
        dy = 1.0f; // prevent division by 0

    float scalar = (NEAREST_RENDER_DIST + (-1.0f * neg_y_vert.y)) / dy;
    float new_x = neg_y_vert.x + scalar * dx;
    float new_z = neg_y_vert.z + scalar * dz;

    neg_y_vert.x = new_x;
    neg_y_vert.y = NEAREST_RENDER_DIST;
    neg_y_vert.z = new_z;
} 