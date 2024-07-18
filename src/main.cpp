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
    #define LEVEL "lvl/lvldata.bsp"
#elif defined(__linux__)
    #define LEVEL "lvl/lvldata.bsp"
#elif defined(_WIN32)
    #define LEVEL "lvl\\lvldata.bsp"
#else
    #error "Unknown operating system"
#endif


int main() {

    Glimpse::GlimpseLogger logger(new ConsoleOut);

    // Initialize map
    Map map(&logger);
    map.LoadFile(LEVEL);

    Camera camera(0.0f, 0.0f, 0.5f, 0.0f);
    GLLib glHandler("doomy", 1000, 750, logger);

    Glaze::GlazeRenderer glazeRenderer(glHandler);

    RenderHandler renderHandler(camera, 
                                glazeRenderer, 
                                glHandler, 
                                logger);

    CallbackHandler callbackHandler(camera, glHandler);
    InputHandler inputHandler(camera, callbackHandler);
    MinimapHandler minimapHandler(camera, map, glazeRenderer);

    // Main loop
    while (!glHandler.WindowShouldClose()) {

        // TODO: Update rendering backend so that it works on windows
        // TODO: Work to get a framerate counter?  This could be a part of the above todo.
        // TODO: Windows build -- try and isolate to one class and get more information to Sean about the exact error message without being
        //          pointer-ified.  Try and put pointers in main as well?

        // Perform work on keyboard input and poll key events
        inputHandler.PerformKeyAction();

        // Render the scene
        renderHandler.Renderw(map, camera);

        // Draw the minimap
        minimapHandler.Draw();

        // Update the display
        glazeRenderer.UpdateDisplay();
    }

    glHandler.Quit();
}