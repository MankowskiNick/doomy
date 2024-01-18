#include "constants.h"
#include "std_graphics.h"
#include "map.h"
#include "camera.h"

#ifndef MINIMAP_H
#ifdef ENABLE_MINIMAP
#define MINIMAP_H

#define MINIMAP_X 0
#define MINIMAP_Y 0

#define MINIMAP_WIDTH 250
#define MINIMAP_HEIGHT 250
#define MINIMAP_SCALE 5

#define MINIMAP_CAMERA_RADIUS 3
#define MINIMAP_ANGLE_INDICATOR_SIZE 10
#define MINIMAP_VERTEX_RADIUS 2

class MinimapHandler {
    public:
        MinimapHandler(Camera& newCamera, Map& newMap, StdGraphicsHandler& newStdGraphicsHandler);

        ~MinimapHandler();

        void Draw();

    private:
        StdGraphicsHandler* stdGraphicsHandler;
        Camera* camera;
        Map* map;

        void DrawPlayer();

        int MapToScreenX(double x);
        int MapToScreenY(double y);
};

#endif
#endif
