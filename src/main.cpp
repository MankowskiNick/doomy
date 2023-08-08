#include <iostream>

#include "shared_graphics.h"
#include "gllib.h"
#include "callback.h"
#include "input.h"
#include "std_graphics.h"
#include "draw.h"
#include "camera.h"
#include "map.h"
#include "minimap.h"

int main(int argc, char** argv) {

    // Initialize map
    Map map;
    map.LoadFile("lvl/map.dat");

    Camera camera(0.0f, 0.0f, 0.5f, 0.0f);
    GLLib glHandler;
    StdGraphicsHandler stdGraphicsHandler(glHandler);

    // Configure drawing and store a pointer to the window
    ConfigureDraw(camera, stdGraphicsHandler, glHandler);

    CallbackHandler callbackHandler(camera, glHandler);
    InputHandler inputHandler(camera, callbackHandler);
    MinimapHandler minimapHandler(camera, map, stdGraphicsHandler);
    
    // Main loop
    while (!glHandler.WindowShouldClose()) {
        
        // Perform work on keyboard input and poll key events
        inputHandler.PerformKeyAction();

        // Render the scene
        Render(map, camera);

        // Draw the minimap
        minimapHandler.Draw();

        // Update the display
        stdGraphicsHandler.UpdateDisplay();
    }
    exit(EXIT_SUCCESS);
}