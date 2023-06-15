#include <math.h>

#include "common_struct.h"


// Are the vertices in counter clockwise order?  That is, is the slope of v1->v2 < v1->v3?
bool ccw(const Vertex& v1, const Vertex& v2, const Vertex& v3) {
    return (v3.y - v1.y)*(v2.x - v1.x) > (v2.y - v1.y) * (v3.x - v1.x);
}

// This is a bit of a black box to me, I'd like to explore more of the math behind it.
// Found at https://bryceboe.com/2006/10/23/line-segment-intersection-algorithm/
// NOTE: This solution does not produce correct results when the two lines are colinear.
// TODO: Find a more general case?
bool Intersect(const Line& l1, const Line& l2) {
    return ccw(l1.v1, l2.v1, l2.v2) != ccw(l1.v2, l2.v1, l2.v2) 
        && ccw(l1.v1, l1.v2, l2.v1) != ccw(l1.v1, l1.v2, l2.v2);
}

float Dist(const Vertex& v1, const Vertex& v2) {
    return pow(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2), 0.5);
}

float DistFromOrigin(const Vertex& v) {
    Vertex origin = { 0.0f, 0.0f, 0.0f };
    return Dist(origin, v);
}

// Translate source by the scalar * translation
void Translate(Vertex& source, const Vertex& translation, float scalar) {
    source.x += translation.x * scalar;
    source.y += translation.y * scalar;
}

// Rotate a vertex around the origin by rads radians
void Rotate(Vertex& source, float rads) {
    float x = (float)source.x;
    float y = (float)source.y;
    source.x = x * cos(-1.0f * rads) - y * sin(-1.0f * rads);
    source.y = y * cos(-1.0f * rads) + x * sin(-1.0f * rads);
}