
#include <iostream>
#include <vector>
#include <math.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "draw.h"
#include "screen_space_mapper.h"

#define min(a, b) (a < b) ? a : b
#define max(a, b) (a > b) ? a : b

// Initialization code, just in case we end up needing to initialize things other than gl_lib
RenderHandler::RenderHandler(Camera& camera, Glaze::GlazeRenderer& newGlazeRenderer, GLLib& newGlLib) {

    glazeRenderer = &newGlazeRenderer;

    // Initialize a new gllib instance
    gl = &newGlLib;

    // Initialize a viewMap, this is a "worker" class that will translate the map
    viewMap = ViewMap(camera);

    occlusionMapVert = new OcclusionMapVert[gl->GetWidth()];
    occlusionMapHoriz = new OcclusionMapHoriz[MAX_WALLCOUNT];
    ResetOcclusionMap();
}

RenderHandler::~RenderHandler() { 
    // TODO: Delete occlusion maps
}

void RenderHandler::ResetOcclusionMap() {
    for (int i = 0; i < MAX_WALLCOUNT; i++) {
        occlusionMapHoriz[i].left = -1;
        occlusionMapHoriz[i].right = -1;
    }
    for (int i = 0; i < gl->GetWidth(); i++) {
        occlusionMapVert[i].topY = gl->GetHeight() - 1;
        occlusionMapVert[i].bottomY = 0;
        OcclusionMapVert cur = occlusionMapVert[i];
    }
}

// Draw an individual wall
// TODO: Refactor this, it's pretty ugly.
void RenderHandler::DrawWall(Wall& wall) {
        // Don't draw walls that are too close -- this will cause errors
        if (abs(wall.line.v1.y) < ERROR_MARGIN || abs(wall.line.v2.y) < ERROR_MARGIN)
            return;

        // Fix a graphical bug that occurs when one point is behind the player
        if (wall.line.v1.y < NEAREST_RENDER_DIST)
            ClipWall(wall.line.v1, wall.line.v2);
        if (wall.line.v2.y < NEAREST_RENDER_DIST)
            ClipWall(wall.line.v2, wall.line.v1);

        // Calculate info for each vertex in the wall
        int screencol_1 = MapToScreenX(wall.line.v1);
        int screencol_2 = MapToScreenX(wall.line.v2);

        Point v1_top = MapVertToPoint(wall.line.v1, wall.max_height);
        Point v1_bot = MapVertToPoint(wall.line.v1, wall.min_height);
        Point v2_top = MapVertToPoint(wall.line.v2, wall.max_height);
        Point v2_bot = MapVertToPoint(wall.line.v2, wall.min_height);

        Point v1_floor = MapVertToPoint(wall.line.v1, wall.floor_height);
        Point v1_ceil = MapVertToPoint(wall.line.v1, wall.ceiling_height);
        Point v2_floor = MapVertToPoint(wall.line.v2, wall.floor_height);
        Point v2_ceil = MapVertToPoint(wall.line.v2, wall.ceiling_height);

        int vert1_bot_y = MapToScreenY(v1_bot);
        int vert2_bot_y = MapToScreenY(v2_bot); 

        int vert1_top_y = MapToScreenY(v1_top);
        int vert2_top_y = MapToScreenY(v2_top);

        int vert1_floor_y = MapToScreenY(v1_floor);
        int vert2_floor_y = MapToScreenY(v2_floor);

        int vert1_ceil_y = MapToScreenY(v1_ceil);
        int vert2_ceil_y = MapToScreenY(v2_ceil);

        float screenrow_top_stepsize = (float)(vert2_top_y - vert1_top_y) / (float)(screencol_2 - screencol_1);
        float screenrow_bot_stepsize = (float)(vert2_bot_y - vert1_bot_y) / (float)(screencol_2 - screencol_1);

        float screenrow_floor_stepsize = (float)(vert2_floor_y - vert1_floor_y) / (float)(screencol_2 - screencol_1);
        float screenrow_ceil_stepsize = (float)(vert2_ceil_y - vert1_ceil_y) / (float)(screencol_2 - screencol_1); 

        int step = screencol_1 < screencol_2 ? 1 : -1;

        // Make sure we aren't drawing what isn't on the screen, even if it isn't drawn it will still take compute time to loop through.
        int start_col = (screencol_1 < 0) ? 0 : ((screencol_1 > WIDTH - 1) ? WIDTH - 1 : screencol_1);
        int end_col = (screencol_2 < 0) ? 0 : ((screencol_2 > WIDTH - 1) ? WIDTH - 1 : screencol_2);

        for (int col = start_col; col != end_col; col += step) {
            // Skip if we have already finished drawing this column
            if (occlusionMapVert[col].bottomY == -1 || occlusionMapVert[col].topY == -1)
                continue;

            // Make sure we aren't taking extra steps that aren't needed
            if (col < 0)
                col = 0;
            if (col >= WIDTH)
                col = WIDTH - 1;

            if (wall.is_portal) {
                // Render 2 portal segments
                int cur_screenrow_top = (int)(vert1_top_y + (screenrow_top_stepsize * (col - screencol_1)));
                int cur_screenrow_bot = (int)(vert1_bot_y + (screenrow_bot_stepsize * (col - screencol_1)));

                int cur_screenrow_ceil =  (int)(vert1_ceil_y + (screenrow_ceil_stepsize * (col - screencol_1)));
                int cur_screenrow_floor = (int)(vert1_floor_y + (screenrow_floor_stepsize * (col - screencol_1)));

                glazeRenderer->DrawLineVert(col, 
                    cur_screenrow_bot, 
                    max(cur_screenrow_floor, occlusionMapVert[col].bottomY), 
                    wall.color);
                glazeRenderer->DrawLineVert(col, 
                    cur_screenrow_ceil, 
                    min(cur_screenrow_top, occlusionMapVert[col].topY), 
                    wall.color);

                // Mark occlusion map
                occlusionMapVert[col].topY = cur_screenrow_ceil;
                occlusionMapVert[col].bottomY = cur_screenrow_floor;
            }
            else {

                // Render wall
                int cur_screenrow_top = (int)(vert1_top_y + (screenrow_top_stepsize * (col - screencol_1)));
                int cur_screenrow_bot = (int)(vert1_bot_y + (screenrow_bot_stepsize * (col - screencol_1)));
                glazeRenderer->DrawLineVert(col, 
                    max(cur_screenrow_bot, occlusionMapVert[col].bottomY), 
                    min(cur_screenrow_top, occlusionMapVert[col].topY), 
                    wall.color);

                // Mark occlusion map
                occlusionMapVert[col].topY = -1;
                occlusionMapVert[col].bottomY = -1;
            }
        }
}

void RenderHandler::RenderBSPNode(BSP_Tree* bsp_tree) {
    if (bsp_tree == NULL)
        return;
    
    if (bsp_tree->is_subsector) {
        // Process subsector -- floors, sprites, NPCs, etc.
    }

    // Vertex to refer to camera position
    Vertex camera_vertex = {
        .id = -1,
        .x = 0.0f,
        .y = 0.0f
    };

    Wall* cur_wall = viewMap.GetMap()->GetWallById(bsp_tree->wall_id);
    bool is_front_retval = is_front(camera_vertex, cur_wall);
    this->RenderBSPNode(is_front_retval ? bsp_tree->front : bsp_tree->back);
    this->DrawWall(*cur_wall);
    this->RenderBSPNode(is_front_retval ? bsp_tree->back : bsp_tree->front);
}

// Render code
void RenderHandler::Render(Map map, Camera& camera) {

    // Display a gray background - Eventually, I don't think this is needed.
    glazeRenderer->FillScreen(100, 100, 100);

    // TOOD: Rework how we handle this
    viewMap.LoadMap(map);
    viewMap.TranslateMap();

    // Reset occlusion map
    this->ResetOcclusionMap();

    // Render walls
    this->RenderBSPNode(&(map.bsp_tree));
}
