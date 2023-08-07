#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

#include "map.h"
#include "camera.h"
#include "draw.h"
#include "input.h"
#include "callback.h"

int main(int argc, char** argv) {

    // Initialize camera & map
    Camera camera = Camera(0.0f, 0.0f, 0.5f, 0.0f);
    Map map;
    map.LoadFile("lvl/map.dat");

    // Configure callbacks
    CallbackHandler callbackHandler(camera);

    // Configure input
    InputHandler inputHandler(camera, callbackHandler);

    // Configure drawing and store a pointer to the window
    GLFWwindow* window = ConfigureDraw(camera, callbackHandler);

    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        
        // Perform work on keyboard input and poll key events
        inputHandler.PerformKeyAction();

        // Render the scene
        Render(map, camera);
    }

    Destroy();
    exit(EXIT_SUCCESS);
}