
#include <iostream>
#include <vector>
#include <math.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "map.h"
#include "viewmap.h"
#include "camera.h"
#include "geom.h"
#include "bsp_tree.h"
#include "wall_node.h"
#include "input.h"
#include "screen_space_mapper.h"

ViewMap view_map;

GLLib* gl;

Glaze::GlazeRenderer* glazeRenderer;

// Initialization code, just in case we end up needing to initialize things other than gl_lib
void ConfigureDraw(Camera& camera, Glaze::GlazeRenderer& newGlazeRenderer, GLLib& newGlLib) {

    glazeRenderer = &newGlazeRenderer;

    // Initialize a new gllib instance
    gl = &newGlLib;

    // Initialize a view_map, this is a "worker" class that will translate the map
    view_map = ViewMap(camera);
}

// Draw an individual wall
void DrawWall(Wall& wall, bool* column_drawn_status) {
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

        int vert1_bottom_y = MapToScreenY(wall.line.v1);
        int vert2_bottom_y = MapToScreenY(wall.line.v2); 

        int vert1_height = MapToScreenHeight(wall.line.v1, wall.height);
        int vert2_height = MapToScreenHeight(wall.line.v2, wall.height);

        float wallheight_stepsize = (float)(vert2_height - vert1_height) / (float)(screencol_2 - screencol_1);
        float screenrow_stepsize = (float)(vert2_bottom_y - vert1_bottom_y) / (float)(screencol_2 - screencol_1);

        int step = screencol_1 < screencol_2 ? 1 : -1;

        // Make sure we aren't drawing what isn't on the screen, even if it isn't drawn it will still take compute time to loop through.
        int start_col = (screencol_1 < 0) ? 0 : ((screencol_1 > WIDTH - 1) ? WIDTH - 1 : screencol_1);
        int end_col = (screencol_2 < 0) ? 0 : ((screencol_2 > WIDTH - 1) ? WIDTH - 1 : screencol_2);

        for (int col = start_col; col != end_col; col += step) {
            // if (*(column_drawn_status + (col * sizeof(bool))))
            //   continue;
            *(column_drawn_status + (col * sizeof(bool))) = true;
            if (col < 0)
                col = 0;
            if (col >= WIDTH)
                col = WIDTH - 1;
            int cur_wallheight = (int)(vert1_height + (wallheight_stepsize * (col - screencol_1)));
            int cur_screenrow = (int)(vert1_bottom_y + (screenrow_stepsize * (col - screencol_1)));

            glazeRenderer->DrawLineVert(col, cur_screenrow, cur_screenrow + cur_wallheight, wall.color);
        }
}

void InitializeColumnStatus(bool* column_drawn_status) {
    for (int i = 0; i < WIDTH; i++)
        *(column_drawn_status + (i * sizeof(bool))) = false;
}

bool IsFinishedDrawing(bool* column_drawn_status) {
    for (int i = 0; i < WIDTH; i++)
        if (*(column_drawn_status + (i * sizeof(bool))) == false)
            return false;
    return true;
}

// temp debug
void DrawBackToFront(Wall_Node* tail, Map& map, bool* column_drawn_status) {

    // We have reached the end of the linked list
    if (tail  == NULL)
        return;

    // Draw the current wall
    Wall* cur_wall = map.GetWallByID(tail->id);
    DrawWall(*cur_wall, column_drawn_status);


    // Recursively call the next
    DrawBackToFront(tail->previous, map, column_drawn_status);

    // Memory management
    if (tail->previous != NULL) {
        tail->previous->next = NULL;
    }
    delete tail;
}

void DrawOrder(Wall_Node* head, Map& map, bool* column_drawn_status) {

    // We have reached the end of the linked list
    if (head == NULL)
        return;

    // Draw the current wall
    Wall* cur_wall = map.GetWallByID(head->id);
    DrawWall(*cur_wall, column_drawn_status);

    // Recursively call the next DrawOrder
    DrawOrder(head->next, map, column_drawn_status);

    // Memory management
    if (head->previous != NULL) {
        head->previous->next = NULL;
    }
    delete head;

}

// Draw all walls
void DrawWalls(Map& map) {
    bool column_drawn_status[WIDTH];
    InitializeColumnStatus(column_drawn_status);

    Wall_Node* render_head = FindOrder(&map.bsp_tree, map);
    
    Wall_Node* tail = render_head;

    while(tail->next != NULL)
       tail = tail->next;
    DrawBackToFront(tail, map, column_drawn_status);

    //DrawOrder(render_head, map, column_drawn_status);
}

// Render code
void Render(Map map, Camera& camera) {

    // Display a gray background
    glazeRenderer->FillScreen(100, 100, 100);

    // TOOD: Maybe get rid of view_map?
    view_map.LoadMap(map);
    view_map.TranslateMap();
    DrawWalls(map);
}
