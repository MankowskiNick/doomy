#ifndef DRAW_H
#define DRAW_H

#include <iostream>
#include <vector>

#include <Glaze/gllib.h>
#include <Glaze/std_graphics.h>

#include "map.h"
#include "camera.h"
#include "callback.h"

void ConfigureDraw(Camera& camera, Glaze::GlazeRenderer& newGlazeRenderer, GLLib& newGlLib);

void Render(Map map, Camera& camera);

#endif