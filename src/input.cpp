#include <GLFW/glfw3.h>
#include <unordered_map>
#include <functional>

#include "camera.h"

std::unordered_map<int, std::function<void()> > action_map;

// Method to configure key actions. If you want to add a keyboard input, it is added here.
void ConfigureKeyActions(Camera& camera) {
    // Motion
    action_map[GLFW_KEY_W] = [&camera]() { camera.MoveForward(); };
    action_map[GLFW_KEY_S] = [&camera]() { camera.MoveBackward(); };
    action_map[GLFW_KEY_A] = [&camera]() { camera.MoveLeft(); };
    action_map[GLFW_KEY_D] = [&camera]() { camera.MoveRight(); };

    // Exit
    action_map[GLFW_KEY_ESCAPE] = []() { exit(0); };
}


void PerformKeyAction(int key) {
    action_map[key]();
}