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

#define SENSITIVITY 0.01f

Camera camera;
double last_mousex, last_mousey;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int* cur_key = NULL;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        delete cur_key;
        cur_key = NULL;
    } else {
        cur_key = new int;
        *cur_key = key;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.angle = -1 * (float)xpos / (1 / SENSITIVITY);
    while (camera.angle > 2 * M_PI) camera.angle -= 2 * M_PI;
    while (camera.angle < 0) camera.angle += 2 * M_PI;

    last_mousex = xpos;
    last_mousey = ypos;
}

int main(int argc, char** argv) {
    GLLib gl = GLLib();

    gl.Init("doomy", WIDTH, HEIGHT, error_callback, key_callback, mouse_callback);
    gl.BindShader("shaders/shader.vsh", "shaders/shader.fsh");
    InitView(gl);

    // Initialize camera & map
    camera = Camera(0.0f, 0.0f, 0.5f, 0.0f, M_PI / 3);
    Map map;
    map.AddVertex(0, -1.0f, -1.0f, 0.0f);
    map.AddVertex(1, -1.0f, 1.0f, 0.0f);
    map.AddVertex(2, 1.0f, -1.0f, 0.0f);
    map.AddVertex(3, 1.0f, 1.0f, 0.0f);
    map.AddWall(0, 1, 1.0f, 255, 0, 0);
    map.AddWall(0, 2, 1.0f, 255, 255, 255);
    map.AddWall(1, 3, 1.0f, 0, 255, 0);
    map.AddWall(2, 3, 1.0f, 0, 0, 255);

    ConfigureKeyActions(camera);
    
    while (!glfwWindowShouldClose(gl.GetWindow())) {

        // Perform work on keyboard input
        if (cur_key != NULL)
            PerformKeyAction(*cur_key);

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
