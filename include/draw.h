#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"

void InitView(GLLib& gl);

void Render(Map map, Camera& camera, int frameCount);

void Draw(GLLib& gl);