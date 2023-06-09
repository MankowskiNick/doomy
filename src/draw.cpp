
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>

#include "shared_graphics.h"
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "viewmap.h"
#include "camera.h"
#include "std_graphics.h"
#include "geom.h"
#include "bsp_tree.h"
#include "wall_node.h"
#include "input.h"

#define WIDTH_DRAW_SCALAR 200 / tan(FOV)
#define HEIGHT_DRAW_SCALAR WIDTH_DRAW_SCALAR * (float)WIDTH / (float)HEIGHT

ViewMap view_map;

GLLib gl;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Initialization code, just in case we end up needing to initialize things other than gl_lib
GLFWwindow* ConfigureDraw(Camera& camera) {

    // Initialize a new gllib instance
    gl = GLLib();
    gl.Init("doomy", WIDTH, HEIGHT, error_callback, key_callback, mouse_callback);
    gl.BindShader("shaders/shader.vsh", "shaders/shader.fsh");

    // Initialize the view
    InitView(gl);

    // Initialize a view_map, this is a "worker" class that will translate the map
    view_map = ViewMap(camera);

    return gl.GetWindow();
}

// TODO: abstract to minimap utility
// Draw the minimap
void DrawMinimap(Map& map) {
    for (int i = 0; i < map.walls.size(); i++) {
        int x1 = (int)(map.walls[i].line.v1.x * MINIMAP_SCALE) + MINIMAP_X + (MINIMAP_WIDTH / 2);
        int y1 = (int)(map.walls[i].line.v1.y * MINIMAP_SCALE) + MINIMAP_Y + (MINIMAP_HEIGHT / 2);

        int x2 = (int)(map.walls[i].line.v2.x * MINIMAP_SCALE) + MINIMAP_X + (MINIMAP_WIDTH / 2);
        int y2 = (int)(map.walls[i].line.v2.y * MINIMAP_SCALE) + MINIMAP_Y + (MINIMAP_HEIGHT / 2);

        DrawLine(x1, y1, x2, y2, map.walls[i].color[0], map.walls[i].color[1], map.walls[i].color[2]);
    }
    DrawPlayer();
}

// Calculate what column the vertex will be at
int MapToScreenX(const Vertex& vert) {
    float y = (vert.y == 0) ? 1.0f : vert.y; // Prevent division by 0
    return (WIDTH_DRAW_SCALAR * vert.x / y) + (WIDTH / 2);
}

// Calculate what row the vertex will be at
int MapToScreenY(const Vertex& vert) {
    float y = (vert.y == 0) ? 1.0f : vert.y; // Prevent division by 0
    return (HEIGHT_DRAW_SCALAR * vert.z / y) + (HEIGHT / 2);
}

int MapToScreenHeight(const Vertex& vert, float wall_height) {
    Vertex second_vert = {
        .x = vert.x,
        .y = vert.y,
        .z = vert.z + wall_height
    };
    return MapToScreenY(second_vert) - MapToScreenY(vert);
}

void ClipWall(Vertex& neg_y_vert, const Vertex& other) { // TODO: maybe refactor parameters?
    float dx = other.x - neg_y_vert.x;
    float dy = other.y - neg_y_vert.y;
    float dz = other.z - neg_y_vert.z;

    if (dy == 0.0f)
        dy = 1.0f; // prevent division by 0

    float scalar = -1.0f * neg_y_vert.y / dy;
    float new_x = neg_y_vert.x + scalar * dx;
    float new_z = neg_y_vert.z + scalar * dz;

    neg_y_vert.x = new_x;
    neg_y_vert.y = 0.01f;
    neg_y_vert.z = new_z;

} 

// Draw an individual wall
void DrawWall(Wall& wall, bool* column_drawn_status) {
        // Fix a graphical bug that occurs when one point is behind the player
        if (wall.line.v1.y < 0)
            ClipWall(wall.line.v1, wall.line.v2);
        if (wall.line.v2.y < 0)
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
            if (*(column_drawn_status + (col * sizeof(bool))))
               continue;
            *(column_drawn_status + (col * sizeof(bool))) = true;
            if (col < 0)
                col = 0;
            if (col >= WIDTH)
                col = WIDTH - 1;
            int cur_wallheight = (int)(vert1_height + (wallheight_stepsize * (col - screencol_1)));
            int cur_screenrow = (int)(vert1_bottom_y + (screenrow_stepsize * (col - screencol_1)));

            DrawLineVert(col, cur_screenrow, cur_screenrow + cur_wallheight, wall.color);
        }
}

// temp debug
int CountNodes(Wall_Node* node) {
    if (node == NULL) return 0;
    return CountNodes(node->next) + 1;
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

void Destroy() {
    glfwDestroyWindow(gl.GetWindow());
    glfwTerminate();
}

// Draw all walls
void DrawWalls(Map& map) {
    bool column_drawn_status[WIDTH];
    InitializeColumnStatus(column_drawn_status);

    Wall_Node* render_head = FindOrder(&map.bsp_tree, map);
    int node_count = CountNodes(render_head);
    DrawOrder(render_head, map, column_drawn_status);
}

// Render code
void Render(Map map, Camera& camera) {

    // Display a gray background
    FillScreen(100, 100, 100);

    view_map.LoadMap(map);
    view_map.TranslateMap();

    DrawWalls(map);
    DrawMinimap(map);

    Draw(gl);
    glfwSwapBuffers(gl.GetWindow());
}
