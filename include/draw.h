#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <vector>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "map.h"
#include "camera.h"
#include "viewmap.h"

#include "wall_node.h"

struct OcclusionMapVert {
    int topY,bottomY;
};

class RenderHandler {
    public:
        RenderHandler(Camera& camera, Glaze::GlazeRenderer& newGlazeRenderer, GLLib& newGlLib);
        ~RenderHandler();

        void Render(Map map, Camera& camera);

    private:
        OcclusionMapVert* occlusionMap;
        ViewMap viewMap;
        GLLib* gl;
        Glaze::GlazeRenderer* glazeRenderer;

        void DrawWall(Wall& wall);
        void DrawBackToFront(Wall_Node* tail);
        void DrawOrder(Wall_Node* head);
        void DrawWalls();

        void ResetOcclusionMap();
        bool IsFrameDone();
};

#endif