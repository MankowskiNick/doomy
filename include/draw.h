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
    bool covered;
};

struct Visplane {
    int id;
    float height; // what is this for?
    int minx, maxx;
    int color[3];
    int top[WIDTH], bottom[WIDTH];
};

struct SectorNode {
    Visplane* floor;
    Visplane* ceiling;
};

class RenderHandler {
    public:
        RenderHandler(Camera& newCamera, 
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
        Camera* camera;

        std::vector<SectorNode> nodes;

        std::vector<int> drawn_walls;
        
        // Current floor & ceiling planes being processed.
        Visplane* floorPlane;
        Visplane* ceilingPlane;

        void ResetFrameData();
        void RenderBSPNode(BSP_Tree* bsp_tree);

        void DrawVertSurface(Wall& wall);
        void DrawPortal(Wall& wall);
        void DrawSurface(Wall& wall);
        void GetDrawRows(int col,
                            int y_bounds[2], 
                            WallSegment segment, 
                            int& bot_row, 
                            int& top_row);
        void DrawQuad(ScreenCoord quad[4], 
                        WallSegment segment, 
                        int color[3], 
                        int floor_diff[2], 
                        int ceiling_diff[2]);

        void RenderSector(Subsector* sector);
        Visplane* BuildEmptyVisplane(int flat_id);
        void FillSectors();
        void RenderVisplane(Visplane* plane);
        // void BuildVisPlanes(ScreenCoord quad[4], 
        //                             WallSegment segment, 
        //                             Subsector& sector);
        // void UpdatePlanesPostDraw();
        bool IsWallDrawn(int id);

};

#endif