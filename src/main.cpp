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

Camera camera;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char** argv) {
    GLLib gl = GLLib();

    gl.Init("doomy", WIDTH, HEIGHT, error_callback, key_callback, mouse_callback);
    gl.BindShader("shaders/shader.vsh", "shaders/shader.fsh");
    InitView(gl);

    // Initialize camera & map
    camera = Camera(0.0f, 0.0f, 0.5f, 0.0f, M_PI / 3);
    Map map;
    map.LoadFile("lvl/map.dat");

    ConfigureInput(camera);
    
    while (!glfwWindowShouldClose(gl.GetWindow())) {

        // Perform work on keyboard input
        PerformKeyAction();

        // Perform work on pixelData
        Render(map, camera);

        // Draw pixels to the screen
        Draw(gl); 

        glfwPollEvents();
        glfwSwapBuffers(gl.GetWindow());
    }

    glfwDestroyWindow(gl.GetWindow());
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
