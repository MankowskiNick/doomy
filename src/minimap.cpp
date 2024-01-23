#include "minimap.h"

MinimapHandler::MinimapHandler(Camera& newCamera, Map& newMap, Glaze::GlazeRenderer& newGlazeRenderer) {
    camera = &newCamera;
    map = &newMap;
    glazeRenderer = &newGlazeRenderer;
}

MinimapHandler::~MinimapHandler() { }

int MinimapHandler::MapToScreenX(double x) {
    return (int)(x * MINIMAP_SCALE) + MINIMAP_X + (MINIMAP_WIDTH / 2);
}

int MinimapHandler::MapToScreenY(double y) {
    return (int)(y * MINIMAP_SCALE) + MINIMAP_Y + (MINIMAP_HEIGHT / 2);
}

void MinimapHandler::DrawPlayer() {
    int center_x = MapToScreenX(camera->x);
    int center_y = MapToScreenY(camera->y);

    int dx = MINIMAP_ANGLE_INDICATOR_SIZE * cos(camera->angle + (M_PI / 2)); // TODO: abstract away "magic value"
    int dy = MINIMAP_ANGLE_INDICATOR_SIZE * sin(camera->angle + (M_PI / 2));

    glazeRenderer->DrawLine(center_x, center_y, center_x + dx, center_y + dy, BLACK);
    glazeRenderer->DrawCircle(center_x, center_y, MINIMAP_CAMERA_RADIUS, WHITE);
}

// Draw the minimap
void MinimapHandler::Draw() {
    for (int i = 0; i < map->walls.size(); i++) {
        int x1 = MapToScreenX(map->walls[i].line.v1.x);
        int y1 = MapToScreenY(map->walls[i].line.v1.y);

        int x2 = MapToScreenX(map->walls[i].line.v2.x);
        int y2 = MapToScreenY(map->walls[i].line.v2.y);

        glazeRenderer->DrawLine(x1, y1, x2, y2, map->walls[i].color[0], map->walls[i].color[1], map->walls[i].color[2]);
    }

    for (int i = 0; i < map->vertices.size(); i++) {
        int x = MapToScreenX(map->vertices[i].x);
        int y = MapToScreenY(map->vertices[i].y);
        glazeRenderer->DrawCircle(x, y, MINIMAP_VERTEX_RADIUS, BLACK);
    }

    DrawPlayer();
}