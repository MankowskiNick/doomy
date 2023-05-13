#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "gllib.h"
#include "texture.h"
#include "map.h"
#include "camera.h"

#define WIDTH 640
#define HEIGHT 480

void InitView(GLLib& gl);

void Render(Map map, Camera& camera);

void Draw(GLLib& gl);