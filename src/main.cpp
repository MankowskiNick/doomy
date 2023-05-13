#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <random>

#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "draw.h"

#define WIDTH 640
#define HEIGHT 480

#define SENSITIVITY 0.01f

Camera camera;
double last_mousex, last_mousey;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch(key) {
        case GLFW_KEY_W:
            camera.MoveForward();
            break;
        case GLFW_KEY_S:
            camera.MoveBackward();
            break;
        case GLFW_KEY_A:
            camera.MoveLeft();
            break;
        case GLFW_KEY_D:
            camera.MoveRight();
            break;
        default:
            break;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.angle = (float)xpos / (1 / SENSITIVITY);
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
    camera = Camera(0.0f, 0, 0, 0, M_PI / 3);
    Map map;
    map.AddVertex(0, -1.0f, -1.0f);
    map.AddVertex(1, -1.0f, 1.0f);
    map.AddVertex(2, 1.0f, -1.0f);
    map.AddVertex(3, 1.0f, 1.0f);
    map.AddWall(0, 1, 0.0f, 1.0f, 255, 0, 0);
    map.AddWall(0, 2, 0.0f, 1.0f, 255, 255, 255);
    map.AddWall(1, 3, 0.0f, 1.0f, 0, 255, 0);
    map.AddWall(2, 3, 0.0f, 1.0f, 0, 0, 255);
    
    while (!glfwWindowShouldClose(gl.GetWindow())) {

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
