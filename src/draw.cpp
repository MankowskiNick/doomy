
#include <iostream>
#include <vector>
#include <math.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "draw.h"
#include "standard.h"

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
    ResetFrameData();
}

RenderHandler::~RenderHandler() { 
    // TODO: Delete occlusion maps
}

void RenderHandler::ResetFrameData() {
    for (int i = 0; i < gl->GetWidth(); i++) {
        occlusionMap[i].topY = gl->GetHeight() - 1;
        occlusionMap[i].bottomY = 0;
        occlusionMap[i].covered = false;
    }

    for (int i = 0; i < nodes.size(); i++) {
        delete nodes[i].floor;
        delete nodes[i].ceiling;
    }
    nodes.resize(0);

    drawn_walls.resize(0);
}

// void RenderHandler::UpdatePlanesPostDraw() {
//     for (int i = floorPlane->minx; i <= floorPlane->maxx; i++) {
//         floorPlane->top[i] = occlusionMap[i].bottomY;
//     }
    
//     for (int i = ceilingPlane->minx; i <= ceilingPlane->maxx; i++) {
//         ceilingPlane->bottom[i] = occlusionMap[i].topY;
//     }
// }

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

void RenderHandler::GetDrawRows(int col, int y_bounds[2], WallSegment segment, int& bot_row, int& top_row) {
    int cur_top, cur_bot;
    switch(segment) {
        case TOP:
            top_row = min(y_bounds[0], occlusionMap[col].topY);
            bot_row = min(y_bounds[1], occlusionMap[col].topY);

            occlusionMap[col].topY = min(top_row, bot_row);
            break;
        case BOTTOM:
            top_row = max(y_bounds[0], occlusionMap[col].bottomY);
            bot_row = max(y_bounds[1], occlusionMap[col].bottomY);

            occlusionMap[col].bottomY = max(top_row, bot_row);
            break;
        case FULL:
            top_row = min(y_bounds[0], occlusionMap[col].topY);
            bot_row = max(y_bounds[1], occlusionMap[col].bottomY);

            occlusionMap[col].topY = top_row;
            occlusionMap[col].bottomY = bot_row;
            occlusionMap[col].covered = true;
            break;
        default:
            logger->Log("Unknown segment type(SEGMENT_TYPE=" + std::to_string(segment) + ")", Glimpse::FATAL);
    }
}

void RenderHandler::DrawQuad(ScreenCoord quad[4], WallSegment segment, int color[3]) {

    // Ensure we are drawing from right to left
    if (quad[0].x > quad[2].x) {
        swap(quad[0], quad[2]);
        swap(quad[1], quad[3]);
    }

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

    // Don't draw outside of the screen
    int left, right;
    left = max(quad[0].x, 0);
    right = min(quad[2].x, WIDTH);

    // Update minx and maxx of the floor and ceiling visplanes
    floorPlane->minx = min(floorPlane->minx, left);
    floorPlane->maxx = max(floorPlane->maxx, right);
    ceilingPlane->minx = min(ceilingPlane->minx, left);
    ceilingPlane->maxx = max(ceilingPlane->maxx, right);

    // Draw column by column
    for (int col = left; col <= right; col++) {

        // Don't draw columns we have already drawn
        if (occlusionMap[col].covered)
            continue;

        // Calculate difference in x since starting
        int dx = col - quad[0].x;

        // Calculate the bounds of the current row
        int y_bounds[2] = {
            (int)(initial_y_bounds[0] + (dy[0] * dx)),  // Top
            (int)(initial_y_bounds[1] + (dy[1] * dx))   // Bottom
        };

        // Update visplane vertical bounds
        switch(segment) {
            case(FULL):
                if (floorPlane != NULL)
                    floorPlane->bottom[col] = occlusionMap[col].bottomY;
                if (ceilingPlane != NULL)
                    ceilingPlane->top[col] = occlusionMap[col].topY;
                break;
            case(TOP):
                if (ceilingPlane != NULL)
                    ceilingPlane->top[col] = occlusionMap[col].topY;
            case(BOTTOM):
                if (floorPlane != NULL)
                    floorPlane->bottom[col] = occlusionMap[col].bottomY;
        }
        
        // Get the draw rows
        int top_row, bot_row;
        this->GetDrawRows(col, y_bounds, segment, bot_row, top_row);

        switch(segment) {
            case FULL:
                floorPlane->top[col] = bot_row;
                ceilingPlane->bottom[col] = top_row;
                break;
            case TOP:
                ceilingPlane->bottom[col] = top_row;
                break;
            case BOTTOM:
                floorPlane->top[col] = bot_row;
                break;
            default:
                break;
        }

        // Draw column
        glazeRenderer->DrawLineVert(col,
                                    top_row,
                                    bot_row,
                                    color);
    }
}


/**
 * @brief Builds an empty visplane from the specified flat ID.
 * 
 * @param flat_id The ID of the flat.
 * @return A pointer to the newly created visplane.
 */
Visplane* RenderHandler::BuildEmptyVisplane(int flat_id) {
    Flat* flat = worldMap->GetFlatById(flat_id);
    Visplane* plane = new Visplane;
    plane->height = flat->height;
    plane->color[0] = flat->color[0];
    plane->color[1] = flat->color[1];
    plane->color[2] = flat->color[2];
    plane->minx = WIDTH;
    plane->maxx = -1;
    memset(plane->top, HEIGHT, sizeof(plane->top));
    memset(plane->bottom, 0, sizeof(plane->bottom));

    return plane;
}

void RenderHandler::RenderSector(Subsector* sector) {
    floorPlane = this->BuildEmptyVisplane(sector->floor_id);
    ceilingPlane = this->BuildEmptyVisplane(sector->ceiling_id);
    
    SectorNode node = {
        .floor = floorPlane,
        .ceiling = ceilingPlane,
    };
    nodes.push_back(node);

    // Draw walls in sector
    for (int i = 0; i < sector->walls.size(); i++) {

        bool skip = false;
        for (int j = 0; j < this->drawn_walls.size(); j++) {
            if (drawn_walls[j] == sector->walls[i]->id) {
                skip = true;
                break;
            }
        }
        if (skip)
            continue;

        // Get the wall we want to draw
        Wall* draw_wall = worldMap->GetWallById(sector->walls[i]->id);

        drawn_walls.push_back(sector->walls[i]->id);

        // Draw wall
        this->DrawVertSurface(*draw_wall);
    }
}

void RenderHandler::RenderBSPNode(BSP_Tree* bsp_tree) {
    if (bsp_tree == NULL)
        return; // may not need this case

    if (bsp_tree->id >= SUBSECTOR) {
        Subsector* sect = worldMap->GetSectorById(bsp_tree->id);
        if (sect == NULL)
            logger->Log("FATAL ERROR: Sector not found(SECT_ID=" + std::to_string(bsp_tree->id) + ").\n", Glimpse::FATAL);
        
        this->RenderSector(sect);
        return;
    }    
    
    // Vertex to refer to camera position
    Vertex camera_vertex = {
        .id = -1,
        .x = 0.0f,
        .y = 0.0f
    };


    // Get dividing wall
    Wall* div_wall = viewMap.GetMap()->GetWallById(bsp_tree->wall_id);

    // Get division line from world line
    DivLine div_line = GetDivLineFromWorldLine(div_wall->line);

    // Which side of the wall is the player standing on?
    WallSide player_side = VertOnSide(div_line, camera_vertex);

    // Render nodes in the proper order depending on player side
    if (player_side == FRONT) {
        this->RenderBSPNode(bsp_tree->front);
        this->RenderBSPNode(bsp_tree->back);
    } 
    else {
        this->RenderBSPNode(bsp_tree->back);
        this->RenderBSPNode(bsp_tree->front);
    }
}

void RenderHandler::RenderVisplane(Visplane* plane) {

    // Nothing to render
    if (plane == NULL)
        return;

    for (int i = plane->minx - 1; i < plane->maxx + 1; i++)
        glazeRenderer->DrawLineVert(i, plane->top[i], plane->bottom[i], plane->color);
}

void RenderHandler::FillSectors() {

    // TODO: Merge planes?

    for (int i = 0; i < this->nodes.size(); i++) {
        SectorNode node = this->nodes[i];

        // Render visplanes
        this->RenderVisplane(node.floor);
        this->RenderVisplane(node.ceiling);
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
    this->ResetFrameData();

    // Render walls
    this->RenderBSPNode(&(map.bsp_tree));

    // Draw horizontal planes
    this->FillSectors();
}
