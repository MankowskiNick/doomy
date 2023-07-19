#include <iostream>

#include "common_struct.h"
#include "vectmath.h"

Vect2<float> VertToVect2(const Vertex& vert) {
    return Vect2<float> {
        .a = vert.x,
        .b = vert.y
    };
}

bool VertexEquals(const Vertex& v1, const Vertex& v2) {
    if (&v1 == NULL || &v2 == NULL) 
        return false; 
    return (abs(v1.x - v2.x) < ERROR_MARGIN
        && abs(v1.y - v2.y) < ERROR_MARGIN
        && abs(v1.z - v2.z) < ERROR_MARGIN);
}