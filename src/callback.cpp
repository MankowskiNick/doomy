
#include "constants.h"
#include "gllib.h" // this breaks the build
#include "callback.h"
#include "camera.h"

#include <iostream>
#include <GLFW/glfw3.h>


CallbackHandler::CallbackHandler(Camera& newCamera, GLLib& gl) { 
    camera_ptr = &newCamera;
    ConfigureCallbacks(this, gl);
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

CallbackHandler* callbackHandler;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    callbackHandler->KeyBind(window, key, scancode, action, mods);
}
void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    callbackHandler->MouseBind(window, xpos, ypos);
}
void ErrorCallback(int error, const char* description) {
    callbackHandler->ErrorBind(error, description);
}

void ConfigureCallbacks(CallbackHandler* newCallbackHandler, GLLib& gl) {
    callbackHandler = newCallbackHandler;

    gl.BindKeyCallback(KeyCallback);
    gl.BindMouseCallback(MouseCallback);
    gl.BindErrorCallback(ErrorCallback);
}