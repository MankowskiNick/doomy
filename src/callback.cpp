#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

#include "constants.h"
#include "callback.h"
#include "camera.h"

CallbackHandler::CallbackHandler(Camera& newCamera) { 
    camera_ptr = &newCamera;
}

CallbackHandler::~CallbackHandler() { }

void CallbackHandler::KeyBind(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE) {
        delete cur_key;
        cur_key = NULL;
    } else {
        cur_key = new int;
        *cur_key = key;
    }
}

void CallbackHandler::MouseBind(GLFWwindow* window, double xpos, double ypos) {
    camera_ptr->angle = -1 * (float)xpos / (1 / SENSITIVITY);
    while (camera_ptr->angle > 2 * M_PI) camera_ptr->angle -= 2 * M_PI;
    while (camera_ptr->angle < 0) camera_ptr->angle += 2 * M_PI;

    cur_mousex = xpos;
    cur_mousey = ypos;
}

void CallbackHandler::ErrorBind(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

int* CallbackHandler::GetCurKey() const {
    return cur_key;
}