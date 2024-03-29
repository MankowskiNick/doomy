
#include <iostream>
#include <vector>
#include <math.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "draw.h"

#define min(a, b) (a < b) ? a : b
#define max(a, b) (a > b) ? a : b

// Initialization code, just in case we end up needing to initialize things other than gl_lib
RenderHandler::RenderHandler(Camera& camera, 
                                Glaze::GlazeRenderer& newGlazeRenderer, 
                                GLLib& newGlLib,
                                Glimpse::GlimpseLogger& newLogger
) {
    logger = &newLogger;

    glazeRenderer = &newGlazeRenderer;

    // Initialize a new gllib instance
    gl = &newGlLib;

    // Initialize a viewMap, this is a "worker" class that will translate the map
    viewMap = ViewMap(camera);

    occlusionMap = new OcclusionMap[gl->GetWidth()];
    ResetOcclusionMap();
}

RenderHandler::~RenderHandler() { 
    // TODO: Delete occlusion maps
}

void RenderHandler::ResetOcclusionMap() {
    for (int i = 0; i < gl->GetWidth(); i++) {
        occlusionMap[i].topY = gl->GetHeight() - 1;
        occlusionMap[i].bottomY = 0;
    }
}

// Draw an individual wall/portal
void RenderHandler::DrawVertSurface(Wall& wall) {

    // Don't draw walls that are too close -- this will cause errors
    if (abs(wall.line.v1.y) < ERROR_MARGIN || abs(wall.line.v2.y) < ERROR_MARGIN)
        return;
    // Fix a graphical bug that occurs when one point is behind the player
    if (wall.line.v1.y < NEAREST_RENDER_DIST)
        ClipWall(wall.line.v1, wall.line.v2);
    if (wall.line.v2.y < NEAREST_RENDER_DIST)
        ClipWall(wall.line.v2, wall.line.v1);
    

    // Render the appropriate 
    if (wall.type == PORTAL)
        this->DrawPortal(wall);
    else if (wall.type == WALL)
        this->DrawSurface(wall);
    else
        logger->Log("FATAL ERROR: Wall is not of valid type(TYPE=" + std::to_string(wall.type) + ").\n", Glimpse::FATAL);
}

void RenderHandler::DrawPortal(Wall& wall) {

    // Get screen space coords of the top quad
    ScreenCoord top_quad[4] = {
        MapToScreenSpace(wall.line.v1, wall.ceiling_height),    // Bottom left
        MapToScreenSpace(wall.line.v1, wall.max_height),        // Top left
        MapToScreenSpace(wall.line.v2, wall.ceiling_height),    // Bottom right
        MapToScreenSpace(wall.line.v2, wall.max_height)         // Top right
    };
    
    // Get screen space coords of bottom quad
    ScreenCoord bot_quad[4] = {
        MapToScreenSpace(wall.line.v1, wall.min_height),        // Bottom left
        MapToScreenSpace(wall.line.v1, wall.floor_height),      // Top left
        MapToScreenSpace(wall.line.v2, wall.min_height),        // Bottom right
        MapToScreenSpace(wall.line.v2, wall.floor_height)       // Top right
    };

    this->DrawQuad(top_quad, TOP, wall.color);
    this->DrawQuad(bot_quad, BOTTOM, wall.color);

}

void RenderHandler::DrawSurface(Wall& wall) {

    // Get screen space coords of the wall
    ScreenCoord quad[4] = {
        MapToScreenSpace(wall.line.v1, wall.min_height),    // Bottom left
        MapToScreenSpace(wall.line.v1, wall.max_height),        // Top left
        MapToScreenSpace(wall.line.v2, wall.min_height),    // Bottom right
        MapToScreenSpace(wall.line.v2, wall.max_height)         // Top right
    };

    this->DrawQuad(quad, FULL, wall.color);
}

// This is a bit problematic atm
void RenderHandler::GetDrawRows(int col, int y_bounds[2], WallSegment segment, int& bot_row, int& top_row) {
    int cur_top, cur_bot;
    switch(segment) {
        case TOP:
            cur_top = occlusionMap[col].topY;
            top_row = min(y_bounds[0], occlusionMap[col].topY);
            bot_row = min(y_bounds[1], occlusionMap[col].topY);

            // top_row = y_bounds[0];
            // bot_row = y_bounds[1];
            occlusionMap[col].topY = min(top_row, bot_row);
            break;
        case BOTTOM:
            cur_bot = occlusionMap[col].bottomY;
            top_row = max(y_bounds[0], occlusionMap[col].bottomY);
            bot_row = max(y_bounds[1], occlusionMap[col].bottomY);

            // top_row = y_bounds[0];
            // bot_row = y_bounds[1];
            occlusionMap[col].bottomY = max(top_row, bot_row);
            break;
        case FULL:
            top_row = min(y_bounds[0], occlusionMap[col].topY);
            bot_row = max(y_bounds[1], occlusionMap[col].bottomY);

            occlusionMap[col].topY = -1;
            occlusionMap[col].bottomY = -1;
            break;
        default:
            logger->Log("FATAL ERROR: Unknown segment type(SEGMENT_TYPE=" + std::to_string(segment) + ")", Glimpse::FATAL);
    }
}

void RenderHandler::DrawQuad(ScreenCoord quad[4], WallSegment segment, int color[3]) {

    // Calculate change in dy for top and bottom edge
    float dy[2] = {
        (float)(quad[3].y - quad[1].y) / (float)(quad[3].x - quad[1].x),  // Top
        (float)(quad[2].y - quad[0].y) / (float)(quad[2].x - quad[0].x),  // Bottom
    };

    // Get y bounds of the first column
    int initial_y_bounds[2] = {
        quad[1].y,     // Top
        quad[0].y      // Bottom
    };

    // Make sure we are stepping the right directions
    int step = (quad[2].x > quad[0].x) ? 1 : -1;

    // Draw the correct bounds
    int first_col = quad[0].x;
    int last_col = quad[2].x;

    // Draw column by column
    for (int col = first_col; col != last_col; col += step) {

        // Don't draw out of screen bounds
        // TODO: Skip these cases to cut out worthless iterations
        if (col >= WIDTH || col < 0)
            continue;

        // Don't draw columns we have already drawn
        if (occlusionMap[col].bottomY == -1 || occlusionMap[col].topY == -1)
            continue;

        // Calculate difference in x since starting
        int dx = col - first_col;

        // Calculate the bounds of the current row
        int y_bounds[2] = {
            (int)(initial_y_bounds[0] + (dy[0] * dx)),  // Top
            (int)(initial_y_bounds[1] + (dy[1] * dx))   // Bottom
        };

        int top_row, bot_row;
        this->GetDrawRows(col, y_bounds, segment, bot_row, top_row);

        // Draw column
        glazeRenderer->DrawLineVert(col,
                                    top_row,
                                    bot_row,
                                    color);

        glazeRenderer->DrawCircle(10, 10, 5, 255, 0, 0);
    }
}

void RenderHandler::DrawSector(Subsector* sector) {
    for (int i = 0; i < sector->walls.size(); i++) {
        Wall* draw_wall = worldMap->GetWallById(sector->walls[i]->id);
        DrawVertSurface(*draw_wall);
    }
}

void RenderHandler::RenderBSPNode(BSP_Tree* bsp_tree) {
    if (bsp_tree == NULL)
        return; // may not need this case

    if (bsp_tree->id >= SUBSECTOR) {
        Subsector* sect = worldMap->GetSectorById(bsp_tree->id);
        if (sect == NULL)
            logger->Log("FATAL ERROR: Sector not found(SECT_ID=" + std::to_string(bsp_tree->id) + ").\n", Glimpse::FATAL);
        this->DrawSector(sect);
        return;
    }    
    
    // Vertex to refer to camera position
    Vertex camera_vertex = {
        .id = -1,
        .x = 0.0f,
        .y = 0.0f
    };


    Wall* div_wall = viewMap.GetMap()->GetWallById(bsp_tree->wall_id);
    DivLine div_line = GetDivLineFromWorldLine(div_wall->line);
    WallSide player_side = VertOnSide(div_line, camera_vertex);

    if (player_side == FRONT) {
        this->RenderBSPNode(bsp_tree->front);
        this->RenderBSPNode(bsp_tree->back);
    } 
    else {
        this->RenderBSPNode(bsp_tree->back);
        this->RenderBSPNode(bsp_tree->front);
    }
}

// Render code
void RenderHandler::Render(Map map, Camera& camera) {

    // Display a gray background - Eventually, I don't think this is needed.
    glazeRenderer->FillScreen(100, 100, 100);

    // TOOD: Rework how we handle this
    viewMap.LoadMap(map);
    viewMap.TranslateMap();
    worldMap = viewMap.GetMap();

    // Reset occlusion map
    this->ResetOcclusionMap();

    // Render walls
    this->RenderBSPNode(&(map.bsp_tree));
}
