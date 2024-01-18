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
#include "std_graphics.h"

void ConfigureDraw(Camera& camera, StdGraphicsHandler& newStdGraphicsHandler, GLLib& newGlLib);

void Render(Map map, Camera& camera);

#endif