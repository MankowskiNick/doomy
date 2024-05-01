#include <Glaze/std_graphics.h>

#include "screen_space_mapper.h"


void DrawQuadFrame(ScreenCoord quad[4], Glaze::GlazeRenderer* glazeRenderer) {
    // Draw bottom line from bottom left to bottom right
    glazeRenderer->DrawLine(quad[0].x, quad[0].y, quad[2].x, quad[2].y, 255, 255, 255);
    // Draw left side line from bottom left to top left
    glazeRenderer->DrawLine(quad[0].x, quad[0].y, quad[1].x, quad[1].y, 255, 255, 255);
    // Draw top line from top left to top right
    glazeRenderer->DrawLine(quad[1].x, quad[1].y, quad[3].x, quad[3].y, 255, 255, 255);
    // Draw right side line from bottom right to top right
    glazeRenderer->DrawLine(quad[2].x, quad[2].y, quad[3].x, quad[3].y, 255, 255, 255);
    // Draw x in the middle
    glazeRenderer->DrawLine(quad[2].x, quad[2].y, quad[1].x, quad[1].y, 255, 255, 255);
    glazeRenderer->DrawLine(quad[0].x, quad[0].y, quad[3].x, quad[3].y, 255, 255, 255);
}