#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>

#include "camera.h"
#include "input.h"

// Map a key to a function
std::unordered_map<int, std::function<void()> > action_map;

// Current key being pressed, if NULL, no key is pressed
int* cur_key = NULL;

double last_mousex, last_mousey;

// Pointer to the camera object coming from main
Camera* camera_ptr;

// Method to configure key actions. If you want to add a keyboard input, it is added here.
void ConfigureInput(Camera& ref_camera) {
    camera_ptr = &ref_camera;

    // Motion
    action_map[GLFW_KEY_W] = [&ref_camera]() { ref_camera.MoveForward(); };
    action_map[GLFW_KEY_S] = [&ref_camera]() { ref_camera.MoveBackward(); };
    action_map[GLFW_KEY_A] = [&ref_camera]() { ref_camera.MoveLeft(); };
    action_map[GLFW_KEY_D] = [&ref_camera]() { ref_camera.MoveRight(); };

    // Exit
    action_map[GLFW_KEY_ESCAPE] = []() { exit(0); };
}

void PerformKeyAction() {
    if (cur_key != NULL)
        action_map[*cur_key]();
}

// Callback function for GLFW in the event of a keyboard update
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        delete cur_key;
        cur_key = NULL;
    } else {
        cur_key = new int;
        *cur_key = key;
    }
}

// Callback function for GLFW in the event of a mouse update
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    camera_ptr->angle = -1 * (float)xpos / (1 / SENSITIVITY);
    while (camera_ptr->angle > 2 * M_PI) camera_ptr->angle -= 2 * M_PI;
    while (camera_ptr->angle < 0) camera_ptr->angle += 2 * M_PI;

    last_mousex = xpos;
    last_mousey = ypos;
}