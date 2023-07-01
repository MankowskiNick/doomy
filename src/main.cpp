#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

#include "shared_graphics.h"
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "draw.h"
#include "input.h"

int main(int argc, char** argv) {

    // Initialize camera & map
    Camera camera = Camera(0.0f, 0.0f, 0.5f, 0.0f);
    Map map;
    map.LoadFile("lvl/render_test.dat");

    // Configure drawing and store a pointer to the window
    GLFWwindow* window = ConfigureDraw(camera);

    // Configure input
    ConfigureInput(camera);
    
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        
        // Perform work on keyboard input and poll key events
        PerformKeyAction();

        // Render the scene
        Render(map, camera);
    }

    Destroy();
    exit(EXIT_SUCCESS);
}
