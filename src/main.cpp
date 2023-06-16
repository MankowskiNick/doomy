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

    // TODO: write a level editor, might do this in python?

    // Initialize camera & map
    camera = Camera(0.0f, 0.0f, 0.5f, 0.0f, M_PI / 3);
    Map map;
    map.AddVertex(0, -1.0f, -1.0f, 0.0f);
    map.AddVertex(1, -1.0f, 1.0f, 0.0f);
    map.AddVertex(2, 1.0f, -1.0f, 0.0f);
    map.AddVertex(3, 1.0f, 1.0f, 0.0f);

    map.AddVertex(5, -1.0f, -1.0f, 1.0f);
    map.AddVertex(6, -1.0f, 1.0f, 1.0f);
    map.AddVertex(7, 1.0f, -1.0f, 1.0f);
    map.AddVertex(8, 1.0f, 1.0f, 1.0f);

    map.AddWall(0, 1, 0.3f, 255, 0, 0);
    map.AddWall(0, 2, 0.3f, 255, 255, 255);
    map.AddWall(1, 3, 0.3f, 0, 255, 0);
    map.AddWall(2, 3, 0.3f, 0, 0, 255);


    map.AddWall(5, 6, 0.3f, 255, 0, 0);
    map.AddWall(5, 7, 0.3f, 255, 255, 255);
    map.AddWall(6, 8, 0.3f, 0, 255, 0);
    map.AddWall(7, 8, 0.3f, 0, 0, 255);

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
