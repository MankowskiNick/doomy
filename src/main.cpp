#include <iostream>

#include <Glimpse/glimpse.h>
#include <Glimpse/consoleout.h>
#include <Glimpse/status.h>
#include <Glimpse/glerrorcode.h>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "callback.h"
#include "input.h"
#include "draw.h"
#include "camera.h"
#include "map.h"
#include "minimap.h"

#ifdef __APPLE__
    #define LEVEL "lvl/lvldata_old.dat"
#else
    #define LEVEL "level\\lvldata_old.dat"
#endif

int main() {

    Glimpse::GlimpseLogger logger(new ConsoleOut);

    // Initialize map
    Map map(&logger);
    map.LoadFile(LEVEL);

    Camera camera(0.0f, 0.0f, 0.5f, 0.0f);
    GLLib glHandler("doomy", 1000, 750, logger);

    Glaze::GlazeRenderer glazeRenderer(glHandler);

    RenderHandler renderHandler(camera, glazeRenderer, glHandler);

    // Configure drawing and store a pointer to the window
    // ConfigureDraw(camera, glazeRenderer, glHandler);

    CallbackHandler callbackHandler(camera, glHandler);
    InputHandler inputHandler(camera, callbackHandler);
    MinimapHandler minimapHandler(camera, map, glazeRenderer);

    // Main loop
    while (!glHandler.WindowShouldClose()) {
        
        // Perform work on keyboard input and poll key events
        inputHandler.PerformKeyAction();

        // Render the scene
        renderHandler.Render(map, camera);

        // Draw the minimap
        minimapHandler.Draw();

        // Update the display
        glazeRenderer.UpdateDisplay();
    }

    glHandler.Quit();
}