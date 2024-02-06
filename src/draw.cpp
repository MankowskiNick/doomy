
#include <iostream>
#include <vector>
#include <math.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "draw.h"
#include "map.h"
#include "viewmap.h"
#include "camera.h"
#include "geom.h"
#include "bsp_tree.h"
#include "wall_node.h"
#include "input.h"
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

    occlusionMap = new OcclusionMapVert[gl->GetWidth()];
    ResetOcclusionMap();
}

RenderHandler::~RenderHandler() { }

void RenderHandler::ResetOcclusionMap() {
    for (int i = 0; i < gl->GetWidth(); i++) {
        occlusionMap[i].topY = gl->GetHeight() - 1;
        occlusionMap[i].bottomY = 0;
        OcclusionMapVert cur = occlusionMap[i];
    }
}

bool RenderHandler::IsFrameDone() {
    for (int i = 0; i < gl->GetWidth(); i++) {
        if (occlusionMap[i].topY != -1 && occlusionMap[i].bottomY != -1)
            return false;
    }
    return true;
}

// Draw an individual wall
void RenderHandler::DrawWall(Wall& wall) {
        // Don't draw walls that are too close
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
            if (occlusionMap[col].bottomY == -1 || occlusionMap[col].topY == -1)
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
                    max(cur_screenrow_floor, occlusionMap[col].bottomY), 
                    wall.color);
                glazeRenderer->DrawLineVert(col, 
                    cur_screenrow_ceil, 
                    min(cur_screenrow_top, occlusionMap[col].topY), 
                    wall.color);

                // Mark occlusion map
                occlusionMap[col].topY = cur_screenrow_ceil;
                occlusionMap[col].bottomY = cur_screenrow_floor;
            }
            else {

                // Render wall
                int cur_screenrow_top = (int)(vert1_top_y + (screenrow_top_stepsize * (col - screencol_1)));
                int cur_screenrow_bot = (int)(vert1_bot_y + (screenrow_bot_stepsize * (col - screencol_1)));
                glazeRenderer->DrawLineVert(col, 
                    max(cur_screenrow_bot, occlusionMap[col].bottomY), 
                    min(cur_screenrow_top, occlusionMap[col].topY), 
                    wall.color);

                // Mark occlusion map
                occlusionMap[col].topY = -1;
                occlusionMap[col].bottomY = -1;
            }
        }
}

void RenderHandler::DrawOrder(Wall_Node* head) {

    // We have reached the end of the linked list
    if (head == NULL)
        return;

    // The frame is fully rendered, stop drawing
    if (IsFrameDone())
        return;

    // Draw the current wall
    Wall* cur_wall = viewMap.GetMap()->GetWallByID(head->id);
    DrawWall(*cur_wall);

    // Recursively call the next DrawOrder
    DrawOrder(head->next);

    // Memory management
    if (head->previous != NULL) {
        head->previous->next = NULL;
    }
    delete head;

}

// Draw all walls
void RenderHandler::DrawWalls() {
    Wall_Node* render_head = FindOrder(&(viewMap.GetMap())->bsp_tree, *viewMap.GetMap());
    
    Wall_Node* tail = render_head;

    ResetOcclusionMap();
    DrawOrder(render_head);
}

// Render code
void RenderHandler::Render(Map map, Camera& camera) {

    // Display a gray background
    glazeRenderer->FillScreen(100, 100, 100);

    // TOOD: Rework how we handle this
    viewMap.LoadMap(map);
    viewMap.TranslateMap();
    DrawWalls();
}
