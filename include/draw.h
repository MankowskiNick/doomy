#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"
#include "callback.h"

GLFWwindow* ConfigureDraw(Camera& camera, CallbackHandler& newCallbackHandler);

void Render(Map map, Camera& camera);

void Draw();

void Destroy();

#endif