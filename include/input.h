#include <GLFW/glfw3.h>

#include "camera.h"

#define SENSITIVITY 0.01f

void ConfigureInput(Camera& camera);

void PerformKeyAction();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);