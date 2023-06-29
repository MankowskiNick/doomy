#include <math.h>

#include "common_struct.h"
#include "vectmath.h"


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
    Vertex origin = { .x = 0.0f, .y = 0.0f, .z = 0.0f };
    return Dist(origin, v);
}

// Translate source by the scalar * translation
void Translate(Vertex& source, const Vertex& translation, float scalar) {
    source.x += translation.x * scalar;
    source.y += translation.y * scalar;
    source.z += translation.z * scalar;
}

// Rotate a vertex around the origin by rads radians on the xy plane
void Rotate(Vertex& source, float rads) {
    float x = (float)source.x;
    float y = (float)source.y;
    source.x = x * cos(-1.0f * rads) - y * sin(-1.0f * rads);
    source.y = y * cos(-1.0f * rads) + x * sin(-1.0f * rads);
}

// Find the intersection point of two lines
Vertex* FindIntersection(const Line& l1, const Line& l2) {
    if (!Intersect(l1, l2)) 
        return NULL;

    Vect2<float> AB = {
        .a = l1.v1.x - l1.v2.x,
        .b = l1.v1.y - l1.v2.y
    };

    Vect2<float> CD = {
        .a = l2.v1.x - l2.v2.x,
        .b = l2.v1.y - l2.v2.y
    };

    Vect2<float> A = VertToVect2(l1.v1);
    Vect2<float> C = VertToVect2(l2.v1);

    Vect2<float> CminA = Add(C, Scale(A, -1.0f));
    Matrix2x2<float> AB_CD = {AB.a, CD.a, AB.b, CD.b};

    Vect2<float> t = Mult(Inverse(AB_CD), CminA);

    Vect2<float> result = Add(A, Scale(AB, t.a));

    Vertex* intersection = new Vertex;

    intersection->id = -1;
    intersection->x = result.a;
    intersection->y = result.b;
    return intersection;
}


Vertex Midpoint(Wall& wall) {
    return {
        .id = 0,
        .x = (wall.line.v1.x + wall.line.v2.x / 2.0f),
        .y = (wall.line.v1.y + wall.line.v2.y / 2.0f)
    };
}