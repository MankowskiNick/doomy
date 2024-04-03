#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <vector>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "map.h"
#include "camera.h"
#include "viewmap.h"
#include "screen_space_mapper.h"

#define MAX_WALLCOUNT 32

struct OcclusionMap {
    int topY,bottomY;
};

class RenderHandler {
    public:
        RenderHandler(Camera& camera, 
                        Glaze::GlazeRenderer& newGlazeRenderer, 
                        GLLib& newGlLib,
                        Glimpse::GlimpseLogger& newLogger);
        ~RenderHandler();

        void Render(Map map, Camera& camera);

    private:
        int walls_drawn = 0;
        OcclusionMap* occlusionMap;
        ViewMap viewMap;
        Map* worldMap;
        GLLib* gl;
        Glaze::GlazeRenderer* glazeRenderer;
        Glimpse::GlimpseLogger* logger;

        void ResetOcclusionMap();
        void RenderBSPNode(BSP_Tree* bsp_tree);

        void DrawVertSurface(Wall& wall);
        void DrawPortal(Wall& wall);
        void DrawSurface(Wall& wall);
        void GetDrawRows(int col,
                            int y_bounds[2], 
                            WallSegment segment, 
                            int& bot_row, 
                            int& top_row);
        void DrawQuad(ScreenCoord* quad, 
                        WallSegment segment, 
                        int color[3]);

        // Currently empty functions - just here to take notes.
        void RenderSector(Subsector* sector);
        void DrawPlanes();
};

#endif