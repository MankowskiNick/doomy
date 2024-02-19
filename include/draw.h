#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <vector>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "map.h"
#include "camera.h"
#include "viewmap.h"

#define MAX_WALLCOUNT 32

struct OcclusionMapVert {
    int topY,bottomY;
};

struct OcclusionMapHoriz {
    int left, right;
};

class RenderHandler {
    public:
        RenderHandler(Camera& camera, Glaze::GlazeRenderer& newGlazeRenderer, GLLib& newGlLib);
        ~RenderHandler();

        void Render(Map map, Camera& camera);

    private:
        int walls_drawn = 0;
        OcclusionMapVert* occlusionMapVert;
        OcclusionMapHoriz* occlusionMapHoriz;
        ViewMap viewMap;
        GLLib* gl;
        Glaze::GlazeRenderer* glazeRenderer;

        void ResetOcclusionMap();
        void RenderBSPNode(BSP_Tree* bsp_tree);
        void DrawWall(Wall& wall);
};

#endif