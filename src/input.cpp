#include <GLFW/glfw3.h>

#include "camera.h"
#include "input.h"
#include "callback.h"

InputHandler::InputHandler(Camera& newCamera, CallbackHandler& newCallbackHandler) {
    callbackHandler = &newCallbackHandler;
    camera_ptr = &newCamera;

    this->ConfigureInput(newCamera);
}

InputHandler::~InputHandler() { }

// Method to configure key actions. If you want to add a keyboard input, it is added here.
void InputHandler::ConfigureInput(Camera& ref_camera) {

    // Motion
    action_map[GLFW_KEY_W] = [&ref_camera]() { ref_camera.MoveForward(); };
    action_map[GLFW_KEY_S] = [&ref_camera]() { ref_camera.MoveBackward(); };
    action_map[GLFW_KEY_A] = [&ref_camera]() { ref_camera.MoveLeft(); };
    action_map[GLFW_KEY_D] = [&ref_camera]() { ref_camera.MoveRight(); };

    // Exit
    action_map[GLFW_KEY_ESCAPE] = []() { exit(0); };
}

void InputHandler::PerformKeyAction() {
    int cur_key = callbackHandler->GetCurKey();
    if (cur_key != NO_KEY && action_map.count(cur_key) != 0)
        action_map[cur_key]();

    glfwPollEvents();
}